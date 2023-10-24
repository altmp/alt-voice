#define NOMINMAX

#include <chrono>
#include <fstream>
#include <algorithm>
#include <limits>
#include "CSoundInput.h"
#include <bass_fx.h>

#include "alt-voice.h"
#include <iostream>

// https://github.com/mumble-voip/mumble/pull/5363/files
static short clampFloatSample(float v) {
	return static_cast<short>(std::min(std::max(v, static_cast<float>(std::numeric_limits<short>::min())),
		static_cast<float>(std::numeric_limits<short>::max())));
}

CSoundInput::CSoundInput(int _bitRate) : encoder(new COpusEncoder(SAMPLE_RATE, AUDIO_CHANNELS, _bitRate)), bitrate(_bitRate)
{
	denoiser = rnnoise_create(nullptr);
	opusBuffer = new char[bitrate];
}

CSoundInput::~CSoundInput()
{
	recording = false;

	if (recordChannel)
	{
		BASS_ChannelStop(recordChannel);
		BASS_ChannelFree(recordChannel);
	}
	BASS_RecordFree();

	delete encoder;
	delete opusBuffer;
	rnnoise_destroy(denoiser);

	encoder = nullptr;
	opusBuffer = nullptr;
	denoiser = nullptr;
}

void CSoundInput::SetVolume(float gain)
{
	volume = gain;
	const BASS_BFX_VOLUME VolumeChangeFXParams = { 0, volume };
	BASS_FXSetParameters(LevelVolumeChangeFX, &VolumeChangeFXParams);
	BASS_FXSetParameters(VolumeChangeFX, &VolumeChangeFXParams);
}

float CSoundInput::GetVolume() const
{
	return volume;
}

int CSoundInput::Read(void* data, size_t size)
{
	return 0;
}

float CSoundInput::GetLevel(bool db) const
{
	return db ? micLevelDb : micLevel;
}

void CSoundInput::SetStreamEnabled(bool enabled)
{
	enabled ? BASS_ChannelPlay(recordChannel, FALSE) : BASS_ChannelPause(recordChannel);
}

int CSoundInput::GetNumDevices() const
{
	int numDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (uint32_t i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			!(deviceInfo.flags & BASS_DEVICE_LOOPBACK) &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) == BASS_DEVICE_TYPE_MICROPHONE)
		{
			numDevices++;
		}
	}
	return numDevices;
}

uint32_t CSoundInput::GetDeviceIdFromIndex(int index) const
{
	BASS_DEVICEINFO deviceInfo;
	int indexCounter = 0;
	for (uint32_t i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			!(deviceInfo.flags & BASS_DEVICE_LOOPBACK) &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) == BASS_DEVICE_TYPE_MICROPHONE)
		{
			if (indexCounter == index)
				return i;
			indexCounter++;
		}
	}
	return 0xFFFFFFFF;
}

const char* CSoundInput::GetDeviceName(const uint32_t deviceId) const
{
	BASS_DEVICEINFO deviceInfo;
	const BOOL result = BASS_RecordGetDeviceInfo(deviceId, &deviceInfo);
	return result ? deviceInfo.name : "INVALID DEVICE";
}

const char* CSoundInput::GetDeviceUID(uint32_t deviceId) const
{
	BASS_DEVICEINFO deviceInfo;
	const BOOL result = BASS_RecordGetDeviceInfo(deviceId, &deviceInfo);
	return result ? deviceInfo.driver : "invalid";
}

AltVoiceError CSoundInput::SelectDeviceByUID(const char* uid)
{
	BASS_DEVICEINFO deviceInfo;

	int nextDeviceId = -1;

	if (uid != nullptr)
	{
		for (uint32_t i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
		{
			if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
				!(deviceInfo.flags & BASS_DEVICE_LOOPBACK) &&
				(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) == BASS_DEVICE_TYPE_MICROPHONE)
			{
				if (!strcmp(deviceInfo.driver, uid))
				{
					nextDeviceId = i;
					break;
				}
			}
		}
		if (nextDeviceId == -1)
			uid = nullptr;
	}

	isDefault = !uid;

	if (recordChannel != 0)
		BASS_RecordFree();

	if (!BASS_RecordInit(nextDeviceId))
		return AltVoiceError::DeviceInit;

	recording = true;

	recordChannel = BASS_RecordStart(SAMPLE_RATE, AUDIO_CHANNELS, 0, OnSoundFrame, this);
	BASS_ChannelSetAttribute(recordChannel, BASS_ATTRIB_GRANULE, FRAME_SIZE_SAMPLES);

	levelChannel = BASS_StreamCreate(SAMPLE_RATE, AUDIO_CHANNELS, BASS_STREAM_DECODE, STREAMPROC_PUSH, this);

	// Change input volume
	const BASS_BFX_VOLUME VolumeChangeFXParams = { BASS_BFX_CHANALL, volume };

	LevelVolumeChangeFX = BASS_ChannelSetFX(levelChannel, BASS_FX_BFX_VOLUME, 0);
	BASS_FXSetParameters(LevelVolumeChangeFX, &VolumeChangeFXParams);

	VolumeChangeFX = BASS_ChannelSetFX(levelChannel, BASS_FX_BFX_VOLUME, 0);
	BASS_FXSetParameters(VolumeChangeFX, &VolumeChangeFXParams);

	BASS_ChannelSetDSP(recordChannel, NoiseDSP, this, 2); //higher prio called first
	BASS_ChannelSetDSP(recordChannel, NormalizeDSP, this, 1);
	//BASS_ChannelSetDSP(recordChannel, RMSDSP, this, 0); //higher prio called first

	BASS_ChannelStart(levelChannel);
	BASS_ChannelPlay(levelChannel, false);

	if (!recordChannel)
		return AltVoiceError::StartStream;

	deviceLost = false;

	return AltVoiceError::Ok;
}

AltVoiceError CSoundInput::SelectDeviceByUID(const std::string& uuid)
{
	return SelectDeviceByUID(uuid.empty() ? nullptr : uuid.c_str());
}

const char* CSoundInput::GetCurrentDeviceUID() const
{
	BASS_DEVICEINFO deviceInfo;
	const uint32_t currentDevice = BASS_RecordGetDevice();
	const BOOL result = BASS_RecordGetDeviceInfo(currentDevice, &deviceInfo);
	return result ? deviceInfo.driver : "invalid";
}

void CSoundInput::UpdateDevice()
{
	if (!recording || isDefault) return;

	auto currentDeviceUID = GetCurrentDeviceUID();
	
	if(deviceLost)
	{
		SelectDeviceByUID(nullptr);
		return;
	}

	bool deviceAlive = false;
	auto numDevices = GetNumDevices();
	for (int i = 0; i < numDevices; i++)
	{
		const int deviceId = GetDeviceIdFromIndex(i);

		if (strcmp(GetDeviceUID(deviceId), currentDeviceUID) == 0)
		{
			deviceAlive = true;
			break;
		}
	}
	if(!deviceAlive)
	{
		deviceLost = true;
		return;
	}
}

void CSoundInput::RegisterCallback(OnVoiceCallback callback)
{
	VoiceCallback = callback;
}

void CSoundInput::SetNoiseSuppressionEnabled(const bool enabled)
{
	noiseSuppressionEnabled = enabled;
}

BOOL CSoundInput::OnSoundFrame(HRECORD handle, const void* buffer, DWORD length, void* user)
{
	const auto self = static_cast<CSoundInput*>(user);

	if (!self->recording)
		return false;

	for (int i = 0; i < length; i += (FRAME_SIZE_SAMPLES * sizeof(short)))
	{
		self->SoundFrameCaptured(handle, (char*)buffer + i, FRAME_SIZE_SAMPLES * sizeof(short));
	}
	return true;
}

bool CSoundInput::IsNoiseSuppressionEnabled() const
{
	return noiseSuppressionEnabled;
}

void CSoundInput::NoiseSuppressionProcess(void* buffer, DWORD length)
{
	const auto shortSamples = static_cast<short*>(buffer);

	// Convert the 16-bit integer samples to floating-point samples
	for (int i = 0; i < FRAME_SIZE_SAMPLES; i++)
	{
		floatBuffer[i] = static_cast<float>(shortSamples[i]);
	}

	// Pass the floating-point samples to the RNNoise function
	for (int i = 0; i < FRAME_SIZE_SAMPLES; i += RNNoiseFrameSize)
	{
		rnnoise_process_frame(denoiser, floatBuffer + i, floatBuffer + i);
	}

	// Convert the floating-point samples back to 16-bit integer samples
	for (int i = 0; i < FRAME_SIZE_SAMPLES; i++)
	{
		shortSamples[i] = static_cast<int16_t>(clampFloatSample(floatBuffer[i]));
	}
}

void CSoundInput::SetNormalizatonEnabled(bool enabled)
{
	normalizationEnabled = enabled;
}

bool CSoundInput::IsNormalizationEnabled() const
{
	return normalizationEnabled;
}

void CSoundInput::Normalize(void* buffer, DWORD length)
{
	short maxFrame = 0;
	for (int i = 0; i < length; ++i)
	{
		short s = abs(((short*)buffer)[i]);
		if (s > maxFrame)
			maxFrame = s;
	}

	if (normalizeMax == 0.f || maxFrame > normalizeMax || normalizeMax / maxFrame < 0.5)
		normalizeMax = maxFrame;
	else
		normalizeMax = (normalizeMax * (NORMALIZE_FRAME_COUNT - 1) + maxFrame) / NORMALIZE_FRAME_COUNT;

	if (normalizeMax <= 1.f)
		return;

	float gain = MAXSHORT / normalizeMax / 2;
	gain = std::fmin<float, float>(gain, 10);

	for (int i = 0; i < length; ++i)
		((short*)buffer)[i] *= gain;
}

void CSoundInput::NormalizeDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user)
{
	const auto self = static_cast<CSoundInput*>(user);
	if (self->IsNormalizationEnabled())
	{
		for (int i = 0; i < length; i += (FRAME_SIZE_SAMPLES * sizeof(short)))
		{
			self->Normalize((char*)buffer + i, FRAME_SIZE_SAMPLES);
		}
	}
}

void CSoundInput::NoiseDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user)
{
	const auto self = static_cast<CSoundInput*>(user);
	if(self->IsNoiseSuppressionEnabled())
	{
		for (int i = 0; i < length; i += (FRAME_SIZE_SAMPLES * sizeof(short)))
		{
			self->NoiseSuppressionProcess((char*)buffer + i, FRAME_SIZE_SAMPLES);
		}
	}
}

void CSoundInput::SoundFrameCaptured(HRECORD handle, const void* buffer, DWORD length)
{
	// Put available data in the level channel
	BASS_StreamPutData(levelChannel, buffer, length * sizeof(short)); 

	// Get current microphone noise level from level channel
	const DWORD currentMicLevel = BASS_ChannelGetLevel(levelChannel);

	// Get left channel noise level from it (because it's mono so right = left)
	const uint16_t leftChannelLevel = LOWORD(currentMicLevel);

	// Convert to float from 0.f to 1.f
	micLevel = static_cast<float>(leftChannelLevel) / MaxShortFloatValue;

	// Convert level to decibels
	micLevelDb = (micLevel > 0 ? 20 * log10(micLevel) : -HUGE_VAL);
	micLevelDb = std::clamp(micLevelDb, -100.f, 0.f) + 100.f;

	// Remove data from level channel (is there a better way to do it?)
	BASS_ChannelGetData(levelChannel, writableBuffer, length * sizeof(short));

	// Copy mic data to output buffer
	memcpy_s(writableBuffer, FRAME_SIZE_SAMPLES * sizeof(short), buffer, length);

	if (VoiceCallback)
	{
		const int opusBufferSize = encoder->EncodeShort(writableBuffer, FRAME_SIZE_SAMPLES, opusBuffer, bitrate);
		VoiceCallback(opusBuffer, opusBufferSize);
	}
}
