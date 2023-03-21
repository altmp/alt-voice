#pragma once
#include "VoiceError.h"
#include <exception>

class CVoiceException: std::exception
{
	AltVoiceError _exception;
public:
	CVoiceException(AltVoiceError voiceException) : _exception(voiceException) {};
	AltVoiceError GetCode() const
	{
		return _exception;
	}
};