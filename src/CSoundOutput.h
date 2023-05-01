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
	[[nodiscard]] const char* GetDeviceName(int id) const override;
	[[nodiscard]] AltVoiceError SelectDevice(int id) override;
	[[nodiscard]] int GetDevice() const override;

	void SetStreamEnabled(bool enabled) override;
	void SetVolume(float vol) override;
	[[nodiscard]] float GetVolume() const override;
};