#include <Windows.h>
#include <thread>
#include <regex>

#include <alt-voice.h>
#include <iostream>

ISoundInput* soundInput = nullptr;
ISoundOutput* soundOutput = nullptr;
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
	std::vector<std::string> inputDevices;
	AV_Initialize();

	AltVoiceError filterCreateError = AV_CreateAudioFilter(&RadioFilter);

	/*
	auto bqLowEffect = RadioFilter->ApplyBqfEffect(0, 1400, 0, 0.86f, 0, 0, 1);
	auto bqHighEffect = RadioFilter->ApplyBqfEffect(1, 900, 0, 0.83f, 0, 0, 2);
	auto distortionEffect = RadioFilter->ApplyDistortionEffect(0.0f, -2.95f, -0.05f, -0.18f, 1.f, 3);
	*/

	AV_CreateSoundOutput(64000, &soundOutput);

	int numDevices = soundOutput->GetNumDevices();
	printf("number of output devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		const int deviceId = soundOutput->GetDeviceIdFromIndex(i);
		printf("%d: %s[%s]\n", i, soundOutput->GetDeviceName(deviceId), soundOutput->GetDeviceUID(deviceId));
	}

	soundOutput->SelectDeviceByUID("default");
	soundOutput->SetStreamEnabled(true);

	AV_CreateSoundInput(64000, &soundInput);

	numDevices = soundInput->GetNumDevices();
	printf("number of input devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		const int deviceId = soundInput->GetDeviceIdFromIndex(i);
		inputDevices.push_back(soundInput->GetDeviceUID(deviceId));
		printf("%d: %s[%s]\n", i, soundInput->GetDeviceName(deviceId), soundInput->GetDeviceUID(deviceId));
	}

	soundInput->RegisterCallback(AudioThread);
	soundInput->SetVolume(1.0f);
	soundInput->SelectDeviceByUID(nullptr);
	soundInput->SetStreamEnabled(true);
	soundInput->SetNoiseSuppressionEnabled(true);
	soundInput->SetNormalizatonEnabled(true);
	std::cout << soundInput->GetCurrentDeviceUID() << std::endl;

	for(;;)
	{
		int nextDeviceIndex = 0;
		std::cin >> nextDeviceIndex;

		if (nextDeviceIndex < inputDevices.size())
		{
			auto err = soundInput->SelectDeviceByUID(inputDevices[nextDeviceIndex].c_str());
			if (err != AltVoiceError::Ok)
			{
				std::cout << AV_GetVoiceErrorText(err) << std::endl;
			}
			else
			{
				std::cout << soundInput->GetCurrentDeviceUID() << std::endl;
			}
		}
	}
}