#pragma once

#include <unordered_map>
#include <string>

#include "ISoundOutput.h"
#include "VoiceError.h"

class ISoundInput
{
public:
	virtual ~ISoundInput() = default;

	virtual void SetVolume(float vol) = 0;
	[[nodiscard]] virtual float GetVolume() const = 0;

	[[nodiscard]] virtual int Read(void* data, size_t size) = 0;

	[[nodiscard]] virtual float GetLevel(bool db = false) const = 0;

	[[nodiscard]] virtual int GetNumDevices() const = 0;
	[[nodiscard]] virtual uint32_t GetDeviceIdFromIndex(int index) const = 0;
	[[nodiscard]] virtual const char* GetDeviceName(uint32_t deviceId) const = 0;
	[[nodiscard]] virtual const char* GetDeviceUID(uint32_t deviceId) const = 0;
	[[nodiscard]] virtual AltVoiceError SelectDeviceByUID(const char* uuid) = 0;
	[[nodiscard]] virtual AltVoiceError SelectDeviceByUID(const std::string& uuid) = 0;
	[[nodiscard]] virtual const char* GetCurrentDeviceUID() const = 0;

	virtual void UpdateDevice() = 0;

	virtual void RegisterCallback(OnVoiceCallback callback) = 0;

	virtual void SetStreamEnabled(bool enabled) = 0;
	virtual void SetNoiseSuppressionEnabled(bool enabled) = 0;
	[[nodiscard]] virtual bool IsNoiseSuppressionEnabled() const = 0;

	virtual void SetNormalizatonEnabled(bool enabled) = 0;
	[[nodiscard]] virtual bool IsNormalizationEnabled() const = 0;
};