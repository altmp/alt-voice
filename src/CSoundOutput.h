#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include <bass.h>

#include "ISoundIO.h"
#include "VoiceError.h"
#include "COpusDecoder.h"

#include "helpers.h"
using namespace helpers;

class CSoundOutput : public ISoundIO
{
private:
	HSTREAM outputStream = 0;
	COpusDecoder* decoder = nullptr;

public:
	CSoundOutput(int bitrate);
	~CSoundOutput();

	void Write(void* data, size_t size) override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SelectDevice(int id) override;
	int GetDevice() override;

	void SetStreamEnabled(bool enabled) override;
};