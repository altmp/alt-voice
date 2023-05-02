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

	if (filterCallback)
	{
		filterCallback(outputBuffer, FRAME_SIZE_BYTES);
	}

	BASS_StreamPutData(outputStream, outputBuffer, FRAME_SIZE_BYTES);
}

void CSoundOutput::SetStreamEnabled(bool enabled)
{
	enabled ? BASS_ChannelPlay(outputStream, FALSE) : BASS_ChannelPause(outputStream);
}

void CSoundOutput::SetVolume(float vol)
{
}

float CSoundOutput::GetVolume() const
{
	return 0.f;
}

int CSoundOutput::GetNumDevices() const
{
	int numDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (uint32_t i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE)
		{
			numDevices++;
		}
	}
	return numDevices;
}

uint32_t CSoundOutput::GetDeviceIdFromIndex(int index) const
{
	BASS_DEVICEINFO deviceInfo;
	int indexCounter = 0;
	for (uint32_t i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE)
		{
			if (indexCounter == index)
				return i;
			indexCounter++;
		}
	}
	return 0xFFFFFFFF;
}

const char* CSoundOutput::GetDeviceName(const uint32_t deviceId) const
{
	BASS_DEVICEINFO deviceInfo;
	const BOOL result = BASS_RecordGetDeviceInfo(deviceId, &deviceInfo);
	return result ? deviceInfo.name : "INVALID DEVICE";
}

const char* CSoundOutput::GetDeviceUID(uint32_t deviceId) const
{
	BASS_DEVICEINFO deviceInfo;
	const BOOL result = BASS_RecordGetDeviceInfo(deviceId, &deviceInfo);
	return result ? deviceInfo.driver : "invalid";
}

AltVoiceError CSoundOutput::SelectDeviceByUID(const char* uid)
{
	/*BASS_DEVICEINFO deviceInfo;

	int nextDeviceId = -1;

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

	if (strcmp(uid, "default") != 0 && nextDeviceId == -1)
		return AltVoiceError::MissingDevice;

	if (recordChannel == 0)
	{
		if (!BASS_RecordInit(nextDeviceId))
			return AltVoiceError::DeviceInit;
	}
	else
	{
		if (!BASS_RecordSetDevice(nextDeviceId))
			return AltVoiceError::DeviceInit;
	}

	int deviceId = -1;
	BASS_DEVICEINFO deviceInfo;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &deviceInfo); i++)
	{
		if (deviceInfo.flags & BASS_DEVICE_ENABLED &&
			(deviceInfo.flags & BASS_DEVICE_TYPE_MASK) != BASS_DEVICE_TYPE_MICROPHONE &&
			i == id)
		{
			deviceId = id;
		}
	}*/

	if (!BASS_Init(-1, SAMPLE_RATE, 0, nullptr, nullptr))
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

const char* CSoundOutput::GetCurrentDeviceUID() const
{
	/*BASS_DEVICEINFO deviceInfo;
	const uint32_t currentDevice = BASS_ChannelGetDevice();
	const BOOL result = BASS_RecordGetDeviceInfo(currentDevice, &deviceInfo);
	return result ? deviceInfo.driver : "invalid";*/
	return "invalid";
}