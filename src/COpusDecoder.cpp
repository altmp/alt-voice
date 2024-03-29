#include <opus/opus.h>
#include "COpusDecoder.h"
#include "CVoiceException.h"

COpusDecoder::COpusDecoder(int sampleRate, int channels)
{
	int opusErr;
	decoder = opus_decoder_create(sampleRate, channels, &opusErr);
	if (opusErr != OPUS_OK || decoder == nullptr)
		throw CVoiceException(AltVoiceError::OpusDecoderCreateError);
}

COpusDecoder::~COpusDecoder()
{
	opus_decoder_destroy(decoder);
}

int COpusDecoder::DecodeShort(void* opusData, size_t size, void* pcmData, size_t outputSize, bool fec, bool missing)
{
	int frame_size;
	
	if (!missing)
	{
		frame_size = opus_decode(decoder, (const unsigned char*)opusData, size, (opus_int16*)pcmData, outputSize, 0);
	}
	else if (fec)
	{
		frame_size = opus_decode(decoder, (const unsigned char*)opusData, size, (opus_int16*)pcmData, outputSize, 1);
		if (frame_size <= 0)
		{
			frame_size = opus_decode(decoder, nullptr, 0, (opus_int16*)pcmData, outputSize, 0);
		}
	}
	else
	{
		frame_size = opus_decode(decoder, nullptr, 0, (opus_int16*)pcmData, outputSize, 0);
	}

	if (frame_size < 0)
		return 0;
	return frame_size;
}
