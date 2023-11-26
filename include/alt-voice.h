#pragma once
#include "ISoundInput.h"
#include "ISoundOutput.h"
#include "IOpusEncoder.h"
#include "IOpusDecoder.h"
#include "VoiceError.h"

#include "IAudioFilter.h"

constexpr int SAMPLE_RATE = 48000;
constexpr int AUDIO_CHANNELS = 1;
constexpr int FRAME_SIZE_SAMPLES = SAMPLE_RATE / 50;
constexpr int FRAME_SIZE_BYTES = FRAME_SIZE_SAMPLES * sizeof(int16_t);

AltVoiceError AV_Initialize();
AltVoiceError AV_CreateSoundOutput(int bitrate, ISoundOutput** soundOutput);
AltVoiceError AV_CreateSoundInput(int bitrate, ISoundInput** soundInput);
void AV_DestroySoundInput(const ISoundInput* _input);
const char* AV_GetVoiceErrorText(AltVoiceError error);
AltVoiceError AV_CreateOpusEncoder(IOpusEncoder** opusEncoder, int bitRate);
AltVoiceError AV_CreateOpusDecoder(IOpusDecoder** opusDecoder);
void AV_DestroyOpusEncoder(const IOpusEncoder* opusEncoder);
void AV_DestroyOpusDecoder(const IOpusDecoder* opusDecoder);
AltVoiceError AV_CreateAudioFilter(IAudioFilter** audioFilter, int channels);
void AV_DestroyAudioFilter(const IAudioFilter* audioFilter);
