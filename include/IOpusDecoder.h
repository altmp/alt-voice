#pragma once

class IOpusDecoder
{
public:
	virtual int DecodeShort(void* opusBuffer, size_t size, void* pcmData, size_t outputSize, bool fec = true, bool missing = false) = 0;
};