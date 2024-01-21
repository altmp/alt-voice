#pragma once
#include <bass.h>
#include <rnnoise.h>
#include <mutex>

#include "alt-voice.h"
#include "ISoundInput.h"
#include "COpusEncoder.h"
#include "VoiceError.h"

class CSoundInput : public ISoundInput
{
	static constexpr int RNNoiseFrameSize = 480;
	static constexpr float MaxShortFloatValue = 32768.0f;
	static constexpr int NORMALIZE_FRAME_COUNT = 50;
	static constexpr float NORMALIZE_SMOOTHING_FACTOR = 0.05f;

	HRECORD recordChannel = 0;
	COpusEncoder* encoder = nullptr;
	int bitrate;

	float volume = 1.f;
	float micLevel = 0.f;
	float micLevelDb = 0.f;
	float currentNormalizationGain = 1.f;
	bool noiseSuppressionEnabled = false;

	bool normalizationEnabled = false;
	float normalizeMax = 0.f;

	HFX VolumeChangeFX;
	DenoiseState* denoiser;

	float floatBuffer[FRAME_SIZE_SAMPLES];
	char* opusBuffer = nullptr;

	OnVoiceCallback VoiceCallback = nullptr;
	
	bool deviceLost = false;
	bool isDefault = true;

	std::atomic_bool recording = false;
public:
	CSoundInput(int _bitRate);
	~CSoundInput() override;

	void SetVolume(float gain) override;
	[[nodiscard]] float GetVolume() const override;

	void SetStreamEnabled(bool enabled) override;
	int Read(void* data, size_t size) override;

	[[nodiscard]] float GetLevel(bool db) const override;

	[[nodiscard]] int GetNumDevices() const override;
	[[nodiscard]] uint32_t GetDeviceIdFromIndex(int index) const override;
	[[nodiscard]] const char* GetDeviceName(uint32_t deviceId) const override;
	[[nodiscard]] const char* GetDeviceUID(uint32_t deviceId) const override;
	AltVoiceError SelectDeviceByUID(const char* uuid) override;
	AltVoiceError SelectDeviceByUID(const std::string& uuid) override;
	[[nodiscard]] const char* GetCurrentDeviceUID() const override;
	void UpdateDevice() override;

	void RegisterCallback(OnVoiceCallback callback) override;

	void SetNoiseSuppressionEnabled(bool enabled) override;
	[[nodiscard]] bool IsNoiseSuppressionEnabled() const override;
	void NoiseSuppressionProcess(void* buffer, DWORD length);

	void SetNormalizatonEnabled(bool enabled) override;
	[[nodiscard]] bool IsNormalizationEnabled() const override;
	void Normalize(void* buffer, DWORD length);

	void SoundFrameCaptured(HRECORD handle, const void* buffer, DWORD length);
	static BOOL OnSoundFrame(HRECORD handle, const void* buffer, DWORD length, void* user);
	static void NormalizeDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user);
	static void NoiseDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user);
};

