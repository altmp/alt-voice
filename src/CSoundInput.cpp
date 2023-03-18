#include <chrono>
#include <fstream>
#include "CSoundInput.h"
#include "CVoiceException.h"

CSoundInput::CSoundInput(int _bitRate) : encoder(new COpusEncoder(SAMPLE_RATE, AUDIO_CHANNELS, _bitRate)){}

CSoundInput::~CSoundInput()
{
	if (encoder)
		delete encoder;

	BASS_RecordFree();
}

void CSoundInput::SetVolume(float gain)
{
	volume = gain;
}

float CSoundInput::GetVolume()
{
	return volume;
}

int CSoundInput::Read(void* data, size_t size)
{
	int available = BASS_ChannelGetData(recordChannel, nullptr, BASS_DATA_AVAILABLE);
	if (available < FRAME_SIZE_BYTES) 
		return 0;

	int16_t inputData[FRAME_SIZE_SAMPLES];
	if (BASS_ChannelGetData(recordChannel, inputData, FRAME_SIZE_BYTES) != FRAME_SIZE_BYTES)
		return 0;

	GainPCM(inputData, FRAME_SIZE_SAMPLES, volume);

	return encoder->EncodeShort(inputData, FRAME_SIZE_SAMPLES, data, size);
}

void CSoundInput::SetStreamEnabled(bool enabled)
{
	enabled ? BASS_ChannelPlay(recordChannel, FALSE) : BASS_ChannelPause(recordChannel);
}

int CSoundInput::GetNumDevices()
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

char* CSoundInput::GetDeviceName(int id)
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

AltVoiceError CSoundInput::SetDevice(int id)
{
	int deviceId = 0;
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

	recordChannel = BASS_RecordStart(SAMPLE_RATE, AUDIO_CHANNELS, BASS_RECORD_PAUSE, nullptr, this);
	
	if (!recordChannel)
		return AltVoiceError::StartStream;

	return AltVoiceError::Ok;
}

int CSoundInput::GetDevice()
{
	return BASS_RecordGetDevice();
}