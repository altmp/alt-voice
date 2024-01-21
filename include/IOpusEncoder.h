#pragma once

class IOpusEncoder
{
public:
	virtual ~IOpusEncoder() = default;

	virtual int EncodeShort(const void* pcmData, size_t size, void* output, size_t outputSize) = 0;
};
