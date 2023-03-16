#include "alt-voice.h"

#include "CSoundInput.h"
#include "CSoundOutput.h"
#include "COpusEncoder.h"
#include "COpusDecoder.h"
#include "CVoiceException.h"

AltVoiceError AV_Initialize()
{
	BASS_Init(0, 48000, 0, nullptr, nullptr);
	BASS_PluginLoad("bassmix", 0);

	return AltVoiceError::Ok;
}

AltVoiceError AV_CreateSoundInput(int bitrate, ISoundIO** soundInput)
{
	try
	{
		*soundInput = new CSoundInput(bitrate);
	}
	catch (const CVoiceException& e)
	{
		return e.GetCode();
	}

	return AltVoiceError::Ok;
}

AltVoiceError AV_CreateSoundOutput(int bitrate, ISoundIO** soundOutput)
{
	try
	{
		*soundOutput = new CSoundOutput(bitrate);
	}
	catch (const CVoiceException& e)
	{
		return e.GetCode();
	}

	return AltVoiceError::Ok;
}

void AV_DestroySoundInput(ISoundIO* _input)
{
	delete _input;
}

const char * AV_GetVoiceErrorText(AltVoiceError error)
{
	switch (error)
	{
	case AltVoiceError::Ok:
		return "No error";
	case AltVoiceError::OpusEncoderCreateError:
		return "Encoder create error";
	case AltVoiceError::OpusDecoderCreateError:
		return "Decoder create error";
	case AltVoiceError::OpusBitrateSetError:
		return "Opus bitrate set error";
	case AltVoiceError::OpusSignalSetError:
		return "Opus signal set error";
	}
	return "Unknown error";
}

ALT_VOICE_API AltVoiceError AV_CreateOpusEncoder(int sampleRate, int channelsCount, IOpusEncoder** opusEncoder, int bitRate)
{
	try
	{
		IOpusEncoder* encoder = new COpusEncoder(sampleRate, channelsCount, bitRate);
		*opusEncoder = encoder;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException &e)
	{
		return e.GetCode();
	}
}

ALT_VOICE_API AltVoiceError AV_CreateOpusDecoder(int sampleRate, int channelsCount, IOpusDecoder** opusDecoder)
{
	try
	{
		IOpusDecoder* decoder = new COpusDecoder(sampleRate, channelsCount);
		*opusDecoder = decoder;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException & e)
	{
		return e.GetCode();
	}
}

ALT_VOICE_API void AV_DestroyOpusEncoder(IOpusEncoder* opusEncoder)
{
	if (opusEncoder)
		delete opusEncoder;
}

ALT_VOICE_API void AV_DestroyOpusDecoder(IOpusDecoder* opusDecoder)
{
	if (opusDecoder)
		delete opusDecoder;
}
