#pragma once
#include <opus/opus.h>
#include "IOpusEncoder.h"

class COpusEncoder : public IOpusEncoder
{
	OpusEncoder* encoder;
public:
	COpusEncoder(int sampleRate, int channels, int bitRate);
	~COpusEncoder();

	int EncodeShort(void* pcmData, size_t size, void* output, size_t outputSize) override;
};
