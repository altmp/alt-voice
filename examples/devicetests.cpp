#include <Windows.h>
#include <thread>
#include <regex>

#include <alt-voice.h>

ISoundIO* soundInput = nullptr;
ISoundIO* soundOutput = nullptr;
IAudioFilter* RadioFilter = nullptr;

void ApplyFilter(void* buffer, uint32_t length)
{
	RadioFilter->Process(buffer, length);
}

void AudioThread(void* buffer, uint32_t size)
{
	soundOutput->Write(buffer, size, ApplyFilter);
}

int main()
{
	AV_Initialize();

	AltVoiceError filterCreateError = AV_CreateAudioFilter(&RadioFilter);

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

	soundOutput->SelectDevice(-1);
	soundOutput->SetStreamEnabled(true);


	AV_CreateSoundInput(64000, &soundInput);

	numDevices = soundInput->GetNumDevices();
	printf("number of input devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		printf("%d: %s\n", i, soundInput->GetDeviceName(i));
	}

	soundInput->RegisterCallback(AudioThread);
	soundInput->SetVolume(1.0f);
	soundInput->SelectDevice(-1);
	soundInput->SetStreamEnabled(true);
	soundInput->SetNoiseSuppressionEnabled(true);

	for(;;)
	{
		Sleep(1000);
	}
}