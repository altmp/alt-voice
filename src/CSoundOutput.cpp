#include "CSoundOutput.h"

#include "alt-voice.h"
#include "CVoiceException.h"

CSoundOutput::CSoundOutput(int _bitRate) : decoder(new COpusDecoder(SAMPLE_RATE, AUDIO_CHANNELS)) {}
CSoundOutput::~CSoundOutput()
{
	delete decoder;

	BASS_ChannelFree(outputStream);
}

void CSoundOutput::Write(void* data, size_t size, OnVoiceCallback filterCallback)
{
	int16_t outputBuffer[FRAME_SIZE_SAMPLES];
	int len = decoder->DecodeShort(data, size, outputBuffer, FRAME_SIZE_SAMPLES, true, false);

	filterCallback(outputBuffer, FRAME_SIZE_BYTES);

	BASS_StreamPutData(outputStream, outputBuffer, FRAME_SIZE_BYTES);
}

void CSoundOutput::SetStreamEnabled(bool enabled)
{
	enabled ? BASS_ChannelPlay(outputStream, FALSE) : BASS_ChannelPause(outputStream);
}

int CSoundOutput::GetNumDevices()
{
	int numDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_GetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE)
		{
			numDevices++;
		}
	}
	return numDevices;
}

char* CSoundOutput::GetDeviceName(int id)
{
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_GetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE &&
			i == id)
		{
			return (char*)deviceInfo.name;
		}
	}
	return nullptr;
}

AltVoiceError CSoundOutput::SelectDevice(int id)
{
	int deviceId = 0;
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE &&
			i == id)
		{
			deviceId = id;
		}
	}

	if (!BASS_Init(deviceId, SAMPLE_RATE, 0, 0, nullptr))
	{
		if(BASS_ErrorGetCode() != BASS_ERROR_ALREADY)
			return AltVoiceError::DeviceInit;
	}

	if (outputStream)
		BASS_ChannelFree(outputStream);

	outputStream = BASS_StreamCreate(SAMPLE_RATE, AUDIO_CHANNELS, 0, STREAMPROC_PUSH, nullptr);
	if (!outputStream)
		return AltVoiceError::StartStream;

	return AltVoiceError::Ok;
}

int CSoundOutput::GetDevice()
{
	return 1;
}