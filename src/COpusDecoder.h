#pragma once
#include <opus/opus.h>
#include "IOpusDecoder.h"

class COpusDecoder : public IOpusDecoder
{
	OpusDecoder* decoder;
public:
	COpusDecoder(int sampleRate, int channels);
	~COpusDecoder() override;

	int DecodeShort(void* opusData, size_t size, void* pcmData, size_t outputSize, bool fec = true, bool missing = false) override;
};
