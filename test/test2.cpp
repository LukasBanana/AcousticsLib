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
    audioSystem->PlaySound(
        "shutter.wav", 1.0f, 0,
        [](Ac::Sound& s)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            
            #if 1
            
            s.SetVolume(s.GetVolume() - 0.01f);
            std::cout << "Playing: " << s.GetSeek() << " / " << s.TotalTime() << std::endl;
            
            #else

            static float a;
            a += 0.05f;
            auto balance = std::sin(a);
            
            s.SetPosition({ balance, 0.0f, -std::sqrt(1.0f - balance*balance) });
            
            std::cout << "Balance: " << balance;
            if (balance < -0.5f)
                std::cout << ", Left";
            else if (balance > 0.5f)
                std::cout << ", Right";
            else
                std::cout << ", Center";
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
