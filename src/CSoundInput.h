#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include <bass.h>
#include <bassmix.h>

#include "ISoundIO.h"
#include "COpusEncoder.h"
#include "VoiceError.h"

#include "helpers.h"
using namespace helpers;

class CSoundInput : public ISoundIO
{
	int deviceId = 0;
	int deviceIdx = 0;
	HRECORD recordChannel = 0;
	COpusEncoder* encoder = nullptr;

	float volume = 1.f;


public:
	CSoundInput(int _bitRate);
	~CSoundInput();

	void SetVolume(float gain) override;
	float GetVolume() override;

	void SetStreamEnabled(bool enabled) override;
	int Read(void* data, size_t size) override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SetDevice(int id) override;
	int GetDevice() override;

	OnVoiceCallback cb = nullptr;
};

