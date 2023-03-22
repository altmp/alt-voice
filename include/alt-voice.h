#pragma once
#include "ISoundIO.h"
#include "IOpusEncoder.h"
#include "IOpusDecoder.h"
#include "VoiceError.h"

#include "IAudioFilter.h"

constexpr int SAMPLE_RATE = 48000;
constexpr int AUDIO_CHANNELS = 1;
constexpr int FRAME_SIZE_SAMPLES = SAMPLE_RATE / 10;
constexpr int FRAME_SIZE_BYTES = FRAME_SIZE_SAMPLES * sizeof(int16_t);

#ifndef ALT_VOICE_API
    #if defined(ALT_LIB_STATIC)
        #define ALT_VOICE_API
    #elif defined(_WIN32)
        #if defined(ALT_VOICE_LIB)
            #define ALT_VOICE_API __declspec(dllexport)
        #else
            #define ALT_VOICE_API __declspec(dllimport)
        #endif
    #else
        #define ALT_VOICE_API extern
    #endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif

ALT_VOICE_API AltVoiceError AV_Initialize();
ALT_VOICE_API AltVoiceError AV_CreateSoundOutput(int bitrate, ISoundIO** soundOutput);
ALT_VOICE_API AltVoiceError AV_CreateSoundInput(int bitrate, ISoundIO** soundInput);
ALT_VOICE_API void AV_DestroySoundInput(ISoundIO* input);
ALT_VOICE_API const char* AV_GetVoiceErrorText(AltVoiceError error);
ALT_VOICE_API AltVoiceError AV_CreateOpusEncoder(IOpusEncoder** opusEncoder, int bitRate);
ALT_VOICE_API AltVoiceError AV_CreateOpusDecoder(IOpusDecoder** opusDecoder);
ALT_VOICE_API void AV_DestroyOpusEncoder(IOpusEncoder* opusEncoder);
ALT_VOICE_API void AV_DestroyOpusDecoder(IOpusDecoder* opusDecoder);
ALT_VOICE_API AltVoiceError AV_CreateAudioFilter(IAudioFilter** audioFilter);
ALT_VOICE_API void AV_DestroyAudioFilter(IAudioFilter* audioFilter);

#if defined(__cplusplus)
}
#endif