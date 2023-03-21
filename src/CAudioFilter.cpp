#include "CAudioFilter.h"
#include <bass_fx.h>
#include "CVoiceException.h"

CAudioFilter::CAudioFilter(const uint32_t sampleRate, const uint32_t channels, uint32_t flags)
{
	Stream = BASS_StreamCreate(sampleRate, channels, flags, STREAMPROC_PUSH, nullptr);
	if (Stream == NULL)
	{
		throw CVoiceException(AltVoiceError::FilterCreateError);
	}
}

CAudioFilter::~CAudioFilter()
{
	BASS_StreamFree(Stream);
}

uint32_t CAudioFilter::ApplyRotateEffect(float fRate, int priority) const
{
	BASS_BFX_ROTATE effectData;
	effectData.fRate = fRate;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_ROTATE, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyVolumeEffect(float fVolume, int priority) const
{
	BASS_BFX_VOLUME effectData;
	effectData.fVolume = fVolume;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_VOLUME, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyPeakeqEffect(int lBand, float fBandwidth, float fQ, float fCenter, float fGain,
	int priority) const
{
	BASS_BFX_PEAKEQ effectData;
	effectData.lBand = lBand;
	effectData.fBandwidth = fBandwidth;
	effectData.fQ = fQ;
	effectData.fCenter = fCenter;
	effectData.fGain = fGain;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_PEAKEQ, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyDampEffect(float fTarget, float fQuiet, float fRate, float fGain, float fDelay,
	int priority) const
{
	BASS_BFX_DAMP effectData;
	effectData.fTarget = fTarget;
	effectData.fQuiet = fQuiet;
	effectData.fRate = fRate;
	effectData.fGain = fGain;
	effectData.fDelay = fDelay;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_DAMP, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyAutowahEffect(float fDryMix, float fWetMix, float fFeedback, float fRate, float fRange,
	float fFreq, int priority) const
{
	BASS_BFX_AUTOWAH effectData;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fFeedback = fFeedback;
	effectData.fRate = fRate;
	effectData.fRange = fRange;
	effectData.fFreq = fFreq;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_AUTOWAH, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyPhaserEffect(float fDryMix, float fWetMix, float fFeedback, float fRate, float fRange,
	float fFreq, int priority) const
{
	BASS_BFX_PHASER effectData;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fFeedback = fFeedback;
	effectData.fRate = fRate;
	effectData.fRange = fRange;
	effectData.fFreq = fFreq;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_PHASER, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyChorusEffect(float fDryMix, float fWetMix, float fFeedback, float fMinSweep,
	float fMaxSweep, float fRate, int priority) const
{
	BASS_BFX_CHORUS effectData;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fFeedback = fFeedback;
	effectData.fMinSweep = fMinSweep;
	effectData.fMaxSweep = fMaxSweep;
	effectData.fRate = fRate;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_CHORUS, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyDistortionEffect(float fDrive, float fDryMix, float fWetMix, float fFeedback, float fVolume,
	int priority) const
{
	BASS_BFX_DISTORTION effectData;
	effectData.fDrive = fDrive;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fFeedback = fFeedback;
	effectData.fVolume = fVolume;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_DISTORTION, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyCompressor2Effect(float fGain, float fThreshold, float fRatio, float fAttack,
	float fRelease, int priority) const
{
	BASS_BFX_COMPRESSOR2 effectData;
	effectData.fGain = fGain;
	effectData.fThreshold = fThreshold;
	effectData.fRatio = fRatio;
	effectData.fAttack = fAttack;
	effectData.fRelease = fRelease;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_COMPRESSOR2, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyBqfEffect(int lFilter, float fCenter, float fGain, float fBandwidth, float fQ, float fS,
	int priority) const
{
	BASS_BFX_BQF effectData;
	effectData.lFilter = lFilter;
	effectData.fCenter = fCenter;
	effectData.fGain = fGain;
	effectData.fBandwidth = fBandwidth;
	effectData.fQ = fQ;
	effectData.fS = fS;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_BQF, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyEcho4Effect(float fDryMix, float fWetMix, float fFeedback, float fDelay, int priority) const
{
	BASS_BFX_ECHO4 effectData;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fFeedback = fFeedback;
	effectData.fDelay = fDelay;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_ECHO4, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyPitchshiftEffect(float fPitchShift, float fSemitones, long lFFTsize, long lOsamp,
	int priority) const
{
	BASS_BFX_PITCHSHIFT effectData;
	effectData.fPitchShift = fPitchShift;
	effectData.fSemitones = fSemitones;
	effectData.lFFTsize = lFFTsize;
	effectData.lOsamp = lOsamp;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_PITCHSHIFT, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

uint32_t CAudioFilter::ApplyFreeverbEffect(float fDryMix, float fWetMix, float fRoomSize, float fDamp, float fWidth,
	uint32_t lMode, int priority) const
{
	BASS_BFX_FREEVERB effectData;
	effectData.fDryMix = fDryMix;
	effectData.fWetMix = fWetMix;
	effectData.fRoomSize = fRoomSize;
	effectData.fDamp = fDamp;
	effectData.fWidth = fWidth;
	effectData.lMode = lMode;
	effectData.lChannel = BASS_BFX_CHANALL;

	const HFX effectHandle = BASS_ChannelSetFX(Stream, BASS_FX_BFX_FREEVERB, priority);
	BASS_FXSetParameters(effectHandle, &effectData);
	return effectHandle;
}

bool CAudioFilter::RemoveEffect(uint32_t hfxHandler) const
{
	return BASS_ChannelRemoveFX(Stream, hfxHandler);
}

void CAudioFilter::Process(void* buffer, size_t size) const
{
	BASS_StreamPutData(Stream, buffer, static_cast<uint32_t>(size));
	BASS_ChannelGetData(Stream, buffer, static_cast<uint32_t>(size));

	BASS_ChannelSetPosition(Stream, 0, BASS_POS_BYTE);
}

