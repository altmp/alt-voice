#pragma once
#include <cmath>
#include <cstdint>

namespace helpers
{
	static constexpr int SAMPLE_RATE = 48000;
	static constexpr int AUDIO_CHANNELS = 1;
	static constexpr int FRAME_SIZE_SAMPLES = SAMPLE_RATE / 10;
	static constexpr int FRAME_SIZE_BYTES = FRAME_SIZE_SAMPLES * sizeof(int16_t);

	static float GetSignalMultiplierForVolume(float volume)
	{	//https://github.com/almoghamdani/audify/blob/master/src/rt_audio.cpp#L422
		// Explained here: https://stackoverflow.com/a/1165188
		return (std::pow<float>(10, volume) - 1) / (10 - 1);
	}

	static void GainPCM(int16_t* data, size_t framesCount, float gain)
	{
		for (int i = 0; i < framesCount; ++i)
			data[i] *= gain;
	}
}