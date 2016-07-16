/*
 * test1.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <string>
#include <vector>


int main()
{
    for (auto module : std::vector<std::string> { "XAudio2", "OpenAL" })
    {
        try
        {
            // Load audio system from shared library
            auto audioSystem = Ac::AudioSystem::Load(module);

            std::cout << "Audio System: " << audioSystem->GetName() << "; Version: " << audioSystem->GetVersion() << std::endl;

            // Play sound
            auto soundFile = "lighter.wav";
            auto sound = audioSystem->LoadSound(soundFile);

            if (sound)
            {
                sound->SetPitch(0.25f);
                sound->Play();

                while (sound->IsPlaying())
                {
                }
            }
            else
                std::cerr << "failed to load sound: \"" << soundFile << '\"' << std::endl;

            //to be continued ...
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}
