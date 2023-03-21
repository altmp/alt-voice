#include "alt-voice.h"

#include "CAudioFilter.h"
#include "CSoundInput.h"
#include "CSoundOutput.h"
#include "COpusEncoder.h"
#include "COpusDecoder.h"
#include "CVoiceException.h"

#include <bass_fx.h>

AltVoiceError AV_Initialize()
{
	BASS_Init(0, SAMPLE_RATE, 0, nullptr, nullptr);
	BASS_FX_GetVersion();

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

void AV_DestroySoundInput(const ISoundIO* _input)
{
	delete _input;
}

const char * AV_GetVoiceErrorText(const AltVoiceError error)
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
	case AltVoiceError::OpusInbandFECSetError:
		return "Opus inband FEC set error";
	case AltVoiceError::OpusPacketlossSetError:
		return "Opus packetloss set error";
	case AltVoiceError::OpusComplexitySetError:
		return "Opus complexity set error";
	case AltVoiceError::OpusPredictionSetError:
		return "Opus prediction set error";
	case AltVoiceError::OpusLSBDepthSetError:
		return "Opus LSB depth set error";
	case AltVoiceError::OpusForceSetError:
		return "Opus force set error";
	case AltVoiceError::OpusDTXSetError:
		return "Opus DTX set error";
	case AltVoiceError::DeviceInit:
		return "Device init error";
	case AltVoiceError::StartStream:
		return "Start stream error";
	case AltVoiceError::MissingDevice:
		return "Missing device error";
	case AltVoiceError::FilterCreateError:
		return "Filter create error";
	default:
		return "Unknown error";
	}
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

ALT_VOICE_API AltVoiceError AV_CreateAudioFilter(int sampleRate, int channelsCount, int flags, IAudioFilter** audioFilter)
{
	try
	{
		IAudioFilter* filter = new CAudioFilter(sampleRate, channelsCount, flags);
		*audioFilter = filter;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException& e)
	{
		return e.GetCode();
	}
}

ALT_VOICE_API void AV_DestroyOpusEncoder(const IOpusEncoder* opusEncoder)
{
	delete opusEncoder;
}

ALT_VOICE_API void AV_DestroyOpusDecoder(const IOpusDecoder* opusDecoder)
{
	delete opusDecoder;
}

ALT_VOICE_API void AV_DestroyAudioFilter(const IAudioFilter* audioFilter)
{
	delete audioFilter;
}
