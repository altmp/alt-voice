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
	int deviceId;
	int deviceIdx;
	HRECORD recordChannel;
	COpusEncoder* encoder;


public:
	CSoundInput(int _bitRate);
	~CSoundInput();

	void SetVolume(float gain) override;
	void SetStreamEnabled(bool enabled) override;
	int Read(void* data, size_t size) override;

	void RegisterCallback(OnVoiceCallback callback) override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SetDevice(int id) override;
	int GetDevice() override;

	OnVoiceCallback cb = nullptr;
};

