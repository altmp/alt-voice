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
	[[nodiscard]] virtual uint32_t GetDeviceIdFromIndex(int index) const = 0;
	[[nodiscard]] virtual const char* GetDeviceName(uint32_t deviceId) const = 0;
	[[nodiscard]] virtual const char* GetDeviceUID(uint32_t deviceId) const = 0;
	[[nodiscard]] virtual AltVoiceError SelectDeviceByUID(const char* uuid) = 0;
	[[nodiscard]] virtual const char* GetCurrentDeviceUID() const = 0;

	virtual void SetStreamEnabled(bool enabled) = 0;
};