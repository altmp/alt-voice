#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <regex>

#include <bass.h>
#include <bassmix.h>
#include <opus/opus.h>

#include <alt-voice.h>

ISoundIO* soundInput = nullptr;
ISoundIO* soundOutput = nullptr;

void AudioThread()
{
	while (true)
	{
		if (soundInput)
		{
			int16_t buffer[960];
			if (auto len = soundInput->Read(buffer, 960))
				soundOutput->Write(buffer, len);
		}
	}
}

int main()
{
	AV_Initialize();
	AV_CreateSoundOutput(64000, &soundOutput);

	int numDevices = soundOutput->GetNumDevices();
	printf("number of output devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		printf("%d: %s\n", i, soundOutput->GetDeviceName(i));
	}

	soundOutput->SetDevice(3);
	soundOutput->SetStreamEnabled(true);


	AV_CreateSoundInput(64000, &soundInput);

	numDevices = soundInput->GetNumDevices();
	printf("number of input devices: %d\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		printf("%d: %s\n", i, soundInput->GetDeviceName(i));
	}

	soundInput->SetDevice(2);
	soundInput->SetStreamEnabled(true);
	soundInput->SetVolume(0.3f);

	std::thread audioThread(AudioThread);

	while (1)
	{
		Sleep(0);
	}
}