#pragma once

#include <string>
#include <unordered_map>
#include "VoiceError.h"

using OnVoiceCallback = void(*)(void* buffer, uint32_t size);

class ISoundIO
{
public:
	virtual ~ISoundIO() = default;

	virtual void SetVolume(float vol) {}
	virtual float GetVolume() { return 0.f; }

	virtual void Write(void* data, size_t size, OnVoiceCallback filterCallback = nullptr) {}
	virtual int Read(void* data, size_t size) { return false; }

	virtual float GetLevel() { return 0.f; }

	virtual int GetNumDevices() = 0;
	virtual char* GetDeviceName(int id) = 0;
	virtual AltVoiceError SelectDevice(int id) = 0;
	virtual int GetDevice() = 0;

	virtual void RegisterCallback(OnVoiceCallback callback) {}

	virtual void SetStreamEnabled(bool enabled) {}
	virtual void SetNoiseSuppressionEnabled(const bool enabled) {}
};