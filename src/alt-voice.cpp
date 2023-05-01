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

AltVoiceError AV_CreateSoundInput(int bitrate, ISoundInput** soundInput)
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

AltVoiceError AV_CreateSoundOutput(int bitrate, ISoundOutput** soundOutput)
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

void AV_DestroySoundInput(const ISoundInput* _input)
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

AltVoiceError AV_CreateOpusEncoder(IOpusEncoder** opusEncoder, int bitRate)
{
	try
	{
		IOpusEncoder* encoder = new COpusEncoder(SAMPLE_RATE, AUDIO_CHANNELS, bitRate);
		*opusEncoder = encoder;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException &e)
	{
		return e.GetCode();
	}
}

AltVoiceError AV_CreateOpusDecoder(IOpusDecoder** opusDecoder)
{
	try
	{
		IOpusDecoder* decoder = new COpusDecoder(SAMPLE_RATE, AUDIO_CHANNELS);
		*opusDecoder = decoder;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException & e)
	{
		return e.GetCode();
	}
}

AltVoiceError AV_CreateAudioFilter(IAudioFilter** audioFilter)
{
	try
	{
		IAudioFilter* filter = new CAudioFilter(SAMPLE_RATE, AUDIO_CHANNELS, BASS_STREAM_DECODE);
		*audioFilter = filter;
		return AltVoiceError::Ok;
	}
	catch (const CVoiceException& e)
	{
		return e.GetCode();
	}
}

void AV_DestroyOpusEncoder(const IOpusEncoder* opusEncoder)
{
	delete opusEncoder;
}

void AV_DestroyOpusDecoder(const IOpusDecoder* opusDecoder)
{
	delete opusDecoder;
}

void AV_DestroyAudioFilter(const IAudioFilter* audioFilter)
{
	delete audioFilter;
}
