#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include <bass.h>
#include <bassmix.h>

#include "ISoundIO.h"
#include "VoiceError.h"
#include "COpusDecoder.h"

#include "helpers.h"
using namespace helpers;

class CSoundOutput : public ISoundIO
{
private:
	HSTREAM outputStream;
	COpusDecoder* decoder;

public:
	CSoundOutput(int bitrate);
	~CSoundOutput();

	void SetVolume(float volume) override;

	void Write(void* data, size_t size) override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SetDevice(int id) override;
	int GetDevice() override;

	void SetStreamEnabled(bool enabled) override;
};