#pragma once

#include <string>
#include <unordered_map>
#include "VoiceError.h"

using OnVoiceCallback = void(*)(void* buffer, uint32_t size);

class ISoundIO
{
public:
	virtual ~ISoundIO() = default;

	virtual void SetVolume(float vol) = 0;

	virtual void Write(void* data, size_t size) {};
	virtual int Read(void* data, size_t size) { return false; };

	virtual int GetNumDevices() = 0;
	virtual char* GetDeviceName(int id) = 0;
	virtual AltVoiceError SetDevice(int id) = 0;
	virtual int GetDevice() = 0;

	virtual void SetStreamEnabled(bool enabled) {};
};