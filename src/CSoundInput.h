#pragma once
#include <bass.h>
#include <rnnoise.h>

#include "alt-voice.h"
#include "ISoundIO.h"
#include "COpusEncoder.h"
#include "VoiceError.h"

class CSoundInput : public ISoundIO
{
	static constexpr int RNNoiseFrameSize = 480;
	static constexpr float MaxShortFloatValue = 32768.0f;

	HRECORD recordChannel = 0;
	COpusEncoder* encoder = nullptr;

	float volume = 1.f;
	float micLevel = 0;
	bool noiseSuppressionEnabled = false;

	HFX VolumeChangeFX;
	DenoiseState* denoiser;

	float floatBuffer[FRAME_SIZE_SAMPLES];

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

	void SetNoiseSuppressionEnabled(const bool enabled) override;

	void NoiseSuppressionProcess(void* buffer, DWORD length);
};

