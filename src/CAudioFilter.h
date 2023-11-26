#pragma once
#include <bass.h>
#include <cstdint>
#include "IAudioFilter.h"

class CAudioFilter: public IAudioFilter
{
	HSTREAM Stream;
public:
	CAudioFilter(uint32_t sampleRate, uint32_t channels, uint32_t flags);
	~CAudioFilter() override;

	[[nodiscard]] uint32_t ApplyRotateEffect(float fRate, int priority) const override;
	[[nodiscard]] uint32_t ApplyVolumeEffect(float fVolume, int priority, int channel) const override;
	[[nodiscard]] uint32_t ApplyPeakeqEffect(int lBand, float fBandwidth, float fQ, float fCenter, float fGain, int priority) const override;
	[[nodiscard]] uint32_t ApplyDampEffect(float fTarget, float fQuiet, float fRate, float fGain, float fDelay, int priority) const override;
	[[nodiscard]] uint32_t ApplyAutowahEffect(float fDryMix, float fWetMix, float fFeedback, float fRate, float fRange, float fFreq, int priority) const override;
	[[nodiscard]] uint32_t ApplyPhaserEffect(float fDryMix, float fWetMix, float fFeedback, float fRate, float fRange, float fFreq, int priority) const override;
	[[nodiscard]] uint32_t ApplyChorusEffect(float fDryMix, float fWetMix, float fFeedback, float fMinSweep, float fMaxSweep, float fRate, int priority) const override;
	[[nodiscard]] uint32_t ApplyDistortionEffect(float fDrive, float fDryMix, float fWetMix, float fFeedback, float fVolume, int priority) const override;
	[[nodiscard]] uint32_t ApplyCompressor2Effect(float fGain, float fThreshold, float fRatio, float fAttack, float fRelease, int priority) const override;
	[[nodiscard]] uint32_t ApplyBqfEffect(int lFilter, float fCenter, float fGain, float fBandwidth, float fQ, float fS, int priority) const override;
	[[nodiscard]] uint32_t ApplyEcho4Effect(float fDryMix, float fWetMix, float fFeedback, float fDelay, int priority) const override;
	[[nodiscard]] uint32_t ApplyPitchshiftEffect(float fPitchShift, float fSemitones, long lFFTsize, long lOsamp, int priority) const override;
	[[nodiscard]] uint32_t ApplyFreeverbEffect(float fDryMix, float fWetMix, float fRoomSize, float fDamp, float fWidth, uint32_t lMode, int priority) const override;
	[[nodiscard]] bool RemoveEffect(uint32_t hfxHandler) const override;

	void Process(void* buffer, size_t size) const override;
};
