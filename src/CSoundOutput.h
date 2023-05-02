#pragma once
#include <bass.h>

#include "alt-voice.h"
#include "ISoundOutput.h"
#include "VoiceError.h"
#include "COpusDecoder.h"

class CSoundOutput : public ISoundOutput
{
private:
	HSTREAM outputStream = 0;
	COpusDecoder* decoder = nullptr;

public:
	CSoundOutput(int bitrate);
	~CSoundOutput();

	void Write(void* data, size_t size, OnVoiceCallback filterCallback = nullptr) override;

	[[nodiscard]] int GetNumDevices() const override;
	[[nodiscard]] uint32_t GetDeviceIdFromIndex(int index) const override;
	[[nodiscard]] const char* GetDeviceName(uint32_t deviceId) const override;
	[[nodiscard]] const char* GetDeviceUID(uint32_t deviceId) const override;
	AltVoiceError SelectDeviceByUID(const char* uuid) override;
	[[nodiscard]] const char* GetCurrentDeviceUID() const override;

	void SetStreamEnabled(bool enabled) override;
	void SetVolume(float vol) override;
	[[nodiscard]] float GetVolume() const override;
};