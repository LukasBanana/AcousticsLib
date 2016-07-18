/*
 * test2.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <cmath>


int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load();
    
        #if 1
    
        Ac::WaveBuffer buffer;
        //Ac::Synthesizer::InitWaveBuffer(buffer, 2.0, 2);
        //Ac::Synthesizer::GenerateSineWave(buffer, 0.0, 2.0, 0.8, 0.0, 300.0);
        //Ac::Synthesizer::GenerateSineWave(buffer, 1.0, 2.0, 0.2, 0.0, 1500.0);

        /*Ac::Synthesizer::GenerateWave(
            buffer, 0.0, 2.0,
            [](double& sample, unsigned short channel, double phase)
            {
                sample += std::sin(phase*2.0*M_PI*50.0)*(0.6 + std::sin(phase*2.0*M_PI*800.0)*0.4);
            }
        );*/

        std::ifstream inputFile("thorndike.wav", std::ios_base::binary);
        audioSystem->ReadAudioBuffer(Ac::AudioFormats::WAV, inputFile, buffer);
        
        auto outputBuffer = buffer;

        Ac::Synthesizer::GenerateWave(
            outputBuffer,
            [&](double& sample, unsigned short channel, double phase)
            {
                sample = buffer.ReadSample(phase, channel);
            }
        );

        #if 0
        std::ofstream outputFile("synthesized_sound.wav", std::ios_base::binary);
        audioSystem->WriteAudioBuffer(Ac::AudioFormats::WAV, outputFile, outputBuffer);
        #endif

        auto sound = audioSystem->CreateSound(outputBuffer);
    
        if (sound)
        {
            sound->SetVolume(0.25f);
            sound->Play();
        
            while (sound->IsPlaying())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                std::cout << "playing: " << sound->GetSeek() << " / " << sound->TotalTime() << std::endl;
            }
        }
    
        #else
    
        audioSystem->PlaySound(
            "shutter.wav", 1.0f, 0,
            [](Ac::Sound& s)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            
                #if 1
            
                s.SetVolume(s.GetVolume() - 0.01f);
                std::cout << "playing: " << s.GetSeek() << " / " << s.TotalTime() << std::endl;
            
                #else

                static float a;
                a += 0.05f;
                auto balance = std::sin(a);
            
                s.SetPosition({ balance, 0.0f, -std::sqrt(1.0f - balance*balance) });
            
                std::cout << "balance: " << balance;
                if (balance < -0.5f)
                    std::cout << ", left";
                else if (balance > 0.5f)
                    std::cout << ", right";
                else
                    std::cout << ", center";
                std::cout << std::endl;
            
                #endif
            
                return s.GetVolume() > 0.05f;
            }
        );

        #endif
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
