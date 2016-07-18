/*
 * test2.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <cmath>


int main()
{
    auto audioSystem = Ac::AudioSystem::Load();
    
    #if 1
    
    Ac::WaveBuffer buffer;
    Ac::Synthesizer::InitWaveBuffer(buffer, 3.0);
    Ac::Synthesizer::GenerateSineWave(buffer, 0.0, 3.0, 0.8, 0.0, 300.0);
    Ac::Synthesizer::GenerateSineWave(buffer, 1.5, 3.0, 0.2, 0.0, 1500.0);
    //Ac::Synthesizer::GenerateSineWave(buffer, 0.0, 1.5, 0.1, 0.3, 150.0);
    
    auto sound = audioSystem->CreateSound(buffer);
    
    if (sound)
    {
        sound->Play();
        
        while (sound->IsPlaying())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::cout << "playing: " << sound->GetSeek() << " / " << sound->TotalTime() << std::endl;
        }
    }
    
    return 0;
    
    #endif
    
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

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}
