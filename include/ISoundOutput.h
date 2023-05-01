#pragma once

#include <unordered_map>
#include "VoiceError.h"

using OnVoiceCallback = void(*)(void* buffer, uint32_t size);

class ISoundOutput
{
public:
	virtual ~ISoundOutput() = default;

	virtual void SetVolume(float vol) = 0;
	[[nodiscard]] virtual float GetVolume() const = 0;

	virtual void Write(void* data, size_t size, OnVoiceCallback filterCallback = nullptr) = 0;

	[[nodiscard]] virtual int GetNumDevices() const = 0;
	[[nodiscard]] virtual const char* GetDeviceName(int id) const = 0;
	[[nodiscard]] virtual AltVoiceError SelectDevice(int id) = 0;
	[[nodiscard]] virtual int GetDevice() const = 0;

	virtual void SetStreamEnabled(bool enabled) = 0;
};