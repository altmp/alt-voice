#pragma once
#include <bass.h>

#include "ISoundIO.h"
#include "COpusEncoder.h"
#include "VoiceError.h"

class CSoundInput : public ISoundIO
{
	HRECORD recordChannel = 0;
	COpusEncoder* encoder = nullptr;

	float volume = 1.f;
	float micLevel = 0;

	HFX VolumeChangeFX;

public:
	CSoundInput(int _bitRate);
	~CSoundInput() override;

	void SetVolume(float gain) override;
	float GetVolume() override;

	void SetStreamEnabled(bool enabled) override;
	int Read(void* data, const size_t size) override;

	float GetLevel() override;

	int GetNumDevices() override;
	char* GetDeviceName(int id) override;
	AltVoiceError SelectDevice(int id) override;
	int GetDevice() override;

	OnVoiceCallback cb = nullptr;
};

