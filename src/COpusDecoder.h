#pragma once
#include <stdio.h>

#include <opus/opus.h>
#include "IOpusDecoder.h"

class COpusDecoder : public IOpusDecoder
{
	OpusDecoder* decoder;
public:
	COpusDecoder(int sampleRate, int channels);
	~COpusDecoder();

	int DecodeShort(void* opusData, size_t size, void* pcmData, size_t outputSize, bool fec = true, bool missing = false) override;
};
