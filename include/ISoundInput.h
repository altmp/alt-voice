#pragma once

#include <unordered_map>

#include "ISoundOutput.h"
#include "VoiceError.h"

class ISoundInput
{
public:
	virtual ~ISoundInput() = default;

	virtual void SetVolume(float vol) = 0;
	[[nodiscard]] virtual float GetVolume() const = 0;

	[[nodiscard]] virtual int Read(void* data, size_t size) = 0;

	[[nodiscard]] virtual float GetLevel() const = 0;

	[[nodiscard]] virtual int GetNumDevices() const = 0;
	[[nodiscard]] virtual char* GetDeviceName(int id) const = 0;
	[[nodiscard]] virtual AltVoiceError SelectDevice(int id) = 0;
	[[nodiscard]] virtual int GetDevice() const = 0;

	virtual void RegisterCallback(OnVoiceCallback callback) = 0;

	virtual void SetStreamEnabled(bool enabled) = 0;
	virtual void SetNoiseSuppressionEnabled(bool enabled) = 0;
	[[nodiscard]] virtual bool IsNoiseSuppressionEnabled() const = 0;
};