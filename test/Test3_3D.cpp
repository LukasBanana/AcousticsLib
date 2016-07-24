/*
 * Test3_3D.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


int main()
{
    auto audioSystem = Ac::AudioSystem::Load();

    auto sound = audioSystem->LoadSound("in/shutter.wav", Ac::SoundFlags::Enable3D);
    if (sound)
    {
        sound->Enable3D();

        Gs::Vector3f pos(10, 0, 1), prevPos, deltaPos;
        sound->SetPosition(pos);

        sound->Play();

        while (sound->IsPlaying())
        {
            // Move sound through the scene
            pos.x -= 0.2f;
            deltaPos = pos - prevPos;
            prevPos = pos;

            // Set sound position and velocity
            sound->SetPosition(pos);
            sound->SetVelocity(deltaPos);
            
            // Print playing status
            PrintTimeline(*sound);
            SleepFor();
        }
        
        std::cout << std::endl;
    }
    else
        std::cerr << "failed to load sound" << std::endl;

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}
