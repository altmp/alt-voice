#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include <bass.h>
#include <bass_fx.h>

#include "ISoundIO.h"
#include "COpusEncoder.h"
#include "VoiceError.h"

#include "helpers.h"
using namespace helpers;

class CSoundInput : public ISoundIO
{
	HRECORD recordChannel = 0;
	COpusEncoder* encoder = nullptr;

	float volume = 1.f;
	int16_t micLevel = 0;

	HFX VolumeChangeFX;

public:
	CSoundInput(int _bitRate);
	~CSoundInput();

	void SetVolume(float gain) override;
	float GetVolume() override;

	void SetStreamEnabled(bool enabled) override;
	int Read(void* data, size_t size) override;

	int16_t GetLevel() override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SelectDevice(int id) override;
	int GetDevice() override;

	OnVoiceCallback cb = nullptr;
};

