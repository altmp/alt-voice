#include <Windows.h>
#include <thread>
#include <regex>

#include <alt-voice.h>

ISoundIO* soundInput = nullptr;
ISoundIO* soundOutput = nullptr;
IAudioFilter* RadioFilter = nullptr;

//{
//	const HFX LowPassBQF = BASS_ChannelSetFX(recordChannel, BASS_FX_BFX_BQF, 1);
//	const BASS_BFX_BQF LowPassBQFParams = { BASS_BFX_BQF_LOWPASS, 1400, 0, 0.86f, 0, 0, BASS_BFX_CHANALL };
//	BASS_FXSetParameters(LowPassBQF, &LowPassBQFParams);
//}
//{
//	const HFX HighPassBQF = BASS_ChannelSetFX(recordChannel, BASS_FX_BFX_BQF, 2);
//	const BASS_BFX_BQF HighPassBQFParams = { BASS_BFX_BQF_HIGHPASS, 900, 0, 0.83f, 0, 0, BASS_BFX_CHANALL };
//	BASS_FXSetParameters(HighPassBQF, &HighPassBQFParams);
//}
//{
//	const HFX DistortionFX = BASS_ChannelSetFX(recordChannel, BASS_FX_BFX_DISTORTION, 3);
//	const BASS_BFX_DISTORTION DistortionFXParams = { 0.0f, -2.95f, -0.05f, -0.18f, 1.f, BASS_BFX_CHANALL };
//	BASS_FXSetParameters(DistortionFX, &DistortionFXParams);
//}


void ApplyFilter(void* buffer, uint32_t length)
{
	RadioFilter->Process(buffer, length);
}

void AudioThread()
{
	for(;;)
	{
		if (soundInput)
		{
			int16_t buffer[FRAME_SIZE_BYTES / 10];
			if (const auto len = soundInput->Read(buffer, FRAME_SIZE_BYTES / 10))
			{
				soundOutput->Write(buffer, len, ApplyFilter);
			}
		}
	}
}

int main()
{
	AV_Initialize();

	AltVoiceError filterCreateError = AV_CreateAudioFilter(SAMPLE_RATE, AUDIO_CHANNELS, 0, &RadioFilter);

	auto bqLowEffect = RadioFilter->ApplyBqfEffect(0, 1400, 0, 0.86f, 0, 0, 1);
	auto bqHighEffect = RadioFilter->ApplyBqfEffect(1, 900, 0, 0.83f, 0, 0, 2);
	auto distortionEffect = RadioFilter->ApplyDistortionEffect(0.0f, -2.95f, -0.05f, -0.18f, 1.f, 3);

	AV_CreateSoundOutput(64000, &soundOutput);

	int numDevices = soundOutput->GetNumDevices();
	printf("number of output devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		printf("%d: %s\n", i, soundOutput->GetDeviceName(i));
	}

	soundOutput->SelectDevice(4);
	soundOutput->SetStreamEnabled(true);


	AV_CreateSoundInput(64000, &soundInput);

	numDevices = soundInput->GetNumDevices();
	printf("number of input devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		printf("%d: %s\n", i, soundInput->GetDeviceName(i));
	}

	soundInput->SetVolume(1.f);
	soundInput->SelectDevice(2);
	soundInput->SetStreamEnabled(true);

	std::thread audioThread(AudioThread);
	audioThread.join();

	for(;;)
	{
		Sleep(0);
	}
}