/*
 * Test5_Mic.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load();

        auto mic = audioSystem->QueryMicrophone();

        if (mic)
        {
            std::cout << "microphone found" << std::endl;

            for (const auto& dev : mic->QueryDevices())
                std::cout << "  device: \"" << dev.name << '\"' << std::endl;

            std::cout << "start recording" << std::endl;
            mic->Start(Ac::WaveBufferFormat(Ac::sampleRate44kHz, 16, 2), 0.1);//0.25);

            auto startTime = std::chrono::system_clock::now();

            auto sound = audioSystem->CreateSound();

            while (mic->IsRecording())
            {
                auto buffer = mic->ReceivedInput();
                if (buffer)
                {
                    std::cout
                        << "received buffer: samples = " << buffer->GetSampleFrames()
                        << ", duration = " << buffer->GetTotalTime() << "s, queue size = " << sound->GetQueueSize() << std::endl;
                    Ac::Synthesizer::BlurWaveBuffer(*buffer, 0.1, 1.0, 15);
                    sound->QueueBuffer(*buffer);
                    if (sound->GetQueueSize() == 2)
                        sound->Play();
                }

                SleepFor();

                auto now = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() > 5000)
                {
                    std::cout << "stop recording" << std::endl;
                    mic->Stop();
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}
