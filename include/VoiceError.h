#pragma once

enum class AltVoiceError
{
	Ok,

	OpusEncoderCreateError,
	OpusDecoderCreateError,
	OpusBitrateSetError,
	OpusSignalSetError,
	OpusInbandFECSetError,
	OpusPacketlossSetError,
	OpusComplexitySetError,
	OpusPredictionSetError,
	OpusLSBDepthSetError,
	OpusForceSetError,
	OpusDTXSetError,

	DeviceInit,
	StartStream,
	MissingDevice,
	FilterCreateError,
};
