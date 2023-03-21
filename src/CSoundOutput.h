#pragma once
#include <bass.h>

#include "alt-voice.h"
#include "ISoundIO.h"
#include "VoiceError.h"
#include "COpusDecoder.h"

class CSoundOutput : public ISoundIO
{
private:
	HSTREAM outputStream = 0;
	COpusDecoder* decoder = nullptr;

public:
	CSoundOutput(int bitrate);
	~CSoundOutput();

	void Write(void* data, size_t size, OnVoiceCallback filterCallback = nullptr) override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SelectDevice(int id) override;
	int GetDevice() override;

	void SetStreamEnabled(bool enabled) override;
};