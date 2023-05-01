#include <chrono>
#include <fstream>
#include "CSoundInput.h"
#include <bass_fx.h>

#include "alt-voice.h"
#include <iostream>


CSoundInput::CSoundInput(int _bitRate) : encoder(new COpusEncoder(SAMPLE_RATE, AUDIO_CHANNELS, _bitRate)), bitrate(_bitRate)
{
	denoiser = rnnoise_create(nullptr);
	opusBuffer = new char[bitrate];
}

CSoundInput::~CSoundInput()
{
	delete encoder;
	delete opusBuffer;
	rnnoise_destroy(denoiser);
	BASS_RecordFree();
}

void CSoundInput::SetVolume(float gain)
{
	volume = gain;
	const BASS_BFX_VOLUME VolumeChangeFXParams = { 0, volume };
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

float CSoundInput::GetLevel() const
{
	return micLevel;
}

void CSoundInput::SetStreamEnabled(bool enabled)
{
	enabled ? BASS_ChannelPlay(recordChannel, FALSE) : BASS_ChannelPause(recordChannel);
}

int CSoundInput::GetNumDevices() const
{
	int numDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
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

char* CSoundInput::GetDeviceName(int id) const
{
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			!(deviceInfo.flags & BASS_DEVICE_LOOPBACK) &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) == BASS_DEVICE_TYPE_MICROPHONE &&
			i == id)
		{
			return (char*)deviceInfo.name;
		}
	}
	return nullptr;
}

AltVoiceError CSoundInput::SelectDevice(int id)
{
	int deviceId = -1;
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			!(deviceInfo.flags & BASS_DEVICE_LOOPBACK) &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) == BASS_DEVICE_TYPE_MICROPHONE &&
			i == id)
		{
			deviceId = id;
		}
	}

	BASS_RecordFree();

	if (!BASS_RecordInit(deviceId))
		return AltVoiceError::DeviceInit;

	recordChannel = BASS_RecordStart(SAMPLE_RATE, AUDIO_CHANNELS, 0, OnSoundFrame, this);
	BASS_ChannelSetAttribute(recordChannel, BASS_ATTRIB_GRANULE, FRAME_SIZE_SAMPLES);

	// Change input volume
	VolumeChangeFX = BASS_ChannelSetFX(recordChannel, BASS_FX_BFX_VOLUME, 0);
	const BASS_BFX_VOLUME VolumeChangeFXParams = { BASS_BFX_CHANALL, volume };
	BASS_FXSetParameters(VolumeChangeFX, &VolumeChangeFXParams);
	
	if (!recordChannel)
		return AltVoiceError::StartStream;

	return AltVoiceError::Ok;
}

int CSoundInput::GetDevice() const
{
	return BASS_RecordGetDevice();
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
	if (noiseSuppressionEnabled)
	{
		const auto shortSamples = static_cast<short*>(buffer);

		// Convert the 16-bit integer samples to floating-point samples
		for (int i = 0; i < FRAME_SIZE_SAMPLES; i++)
		{
			floatBuffer[i] = static_cast<float>(shortSamples[i]) / MaxShortFloatValue;
		}

		// Pass the floating-point samples to the RNNoise function
		for (int i = 0; i < FRAME_SIZE_SAMPLES; i += RNNoiseFrameSize)
		{
			rnnoise_process_frame(denoiser, floatBuffer + i, floatBuffer + i);
		}

		// Convert the floating-point samples back to 16-bit integer samples
		for (int i = 0; i < FRAME_SIZE_SAMPLES; i++)
		{
			shortSamples[i] = static_cast<int16_t>(floatBuffer[i] * MaxShortFloatValue);
		}
	}
}

void CSoundInput::SoundFrameCaptured(HRECORD handle, const void* buffer, DWORD length)
{
	// Create new buffer on stack because buffer was marked as const in API
	memcpy_s(writableBuffer, FRAME_SIZE_SAMPLES * sizeof(short), buffer, length);

	// Apply noise suppression
	NoiseSuppressionProcess(writableBuffer, FRAME_SIZE_SAMPLES);

	// Get current microphone noise level
	const DWORD currentMicLevel = BASS_ChannelGetLevel(handle);

	// Get left channel noise level from it (because it's mono so right = left)
	const uint16_t leftChannelLevel = LOWORD(currentMicLevel);

	// Convert to float from 0.f to 1.f
	micLevel = static_cast<float>(leftChannelLevel) / MaxShortFloatValue;

	if (VoiceCallback)
	{
		const int opusBufferSize = encoder->EncodeShort(writableBuffer, FRAME_SIZE_SAMPLES, opusBuffer, bitrate);
		VoiceCallback(opusBuffer, opusBufferSize);
	}
}
