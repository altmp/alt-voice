#pragma once
#include "ISoundIO.h"
#include "IOpusEncoder.h"
#include "IOpusDecoder.h"
#include "VoiceError.h"
#include <vector>
#include <string>

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
ALT_VOICE_API AltVoiceError AV_CreateOpusEncoder(int sampleRate, int channelsCount, IOpusEncoder** opusEncoder, int bitRate);
ALT_VOICE_API AltVoiceError AV_CreateOpusDecoder(int sampleRate, int channelsCount, IOpusDecoder** opusDecoder);
ALT_VOICE_API void AV_DestroyOpusEncoder(IOpusEncoder* opusEncoder);
ALT_VOICE_API void AV_DestroyOpusDecoder(IOpusDecoder* opusDecoder);

#if defined(__cplusplus)
}
#endif