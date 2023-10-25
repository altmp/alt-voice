#include <opus/opus.h>
#include "COpusEncoder.h"
#include "CVoiceException.h"

COpusEncoder::COpusEncoder(int sampleRate, int channels, int bitRate)
{
	int opusErr;
	encoder = opus_encoder_create(sampleRate, channels, OPUS_APPLICATION_AUDIO, &opusErr);
	if (opusErr != OPUS_OK || encoder == nullptr)
		throw CVoiceException(AltVoiceError::OpusEncoderCreateError);

	if (opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitRate)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusBitrateSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusSignalSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(1)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusInbandFECSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_PACKET_LOSS_PERC(15)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusPacketlossSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(10)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusComplexitySetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_PREDICTION_DISABLED(0)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusPredictionSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_LSB_DEPTH(16)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusLSBDepthSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_FORCE_CHANNELS(1)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusForceSetError);

	if (opus_encoder_ctl(encoder, OPUS_SET_DTX(0)) != OPUS_OK)
		throw CVoiceException(AltVoiceError::OpusDTXSetError);
}

COpusEncoder::~COpusEncoder()
{
	opus_encoder_destroy(encoder);
}

int COpusEncoder::EncodeShort(void* pcmData, size_t size, void* output, size_t outputSize)
{
	const int len = opus_encode(encoder, (const opus_int16*)pcmData, size, (unsigned char*)output, outputSize);
	if (len < 0 || len > outputSize)
		return 0;
	return len;
}
