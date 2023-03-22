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
	int bitrate;

	float volume = 1.f;
	float micLevel = 0;
	bool noiseSuppressionEnabled = false;

	HFX VolumeChangeFX;
	DenoiseState* denoiser;

	short writableBuffer[FRAME_SIZE_SAMPLES];
	float floatBuffer[FRAME_SIZE_SAMPLES];
	char* opusBuffer = nullptr;

	OnVoiceCallback VoiceCallback = nullptr;

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

	void RegisterCallback(OnVoiceCallback callback) override;

	void SetNoiseSuppressionEnabled(const bool enabled) override;
	void NoiseSuppressionProcess(void* buffer, DWORD length);
	void SoundFrameCaptured(HRECORD handle, const void* buffer, DWORD length);

	static BOOL OnSoundFrame(HRECORD handle, const void* buffer, DWORD length, void* user);
};

