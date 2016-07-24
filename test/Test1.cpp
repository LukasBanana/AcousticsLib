/*
 * Test1.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


int main()
{
    for (auto module : Ac::AudioSystem::FindModules())
    {
        try
        {
            // Load audio system from shared library
            auto audioSystem = Ac::AudioSystem::Load(module);

            std::cout << "Audio System: " << audioSystem->GetName() << "; Version: " << audioSystem->GetVersion() << std::endl;

            // Play sound
            auto soundFile = (
                "in/lighter.wav"
                //"in/Blow.aiff"
            );
            auto sound = audioSystem->LoadSound(soundFile);

            if (sound)
            {
                //sound->SetPitch(0.25f);
                //sound->SetLooping(true);
                sound->Play();

                if (sound->GetLooping())
                {
                    bool checkRep = false;
                    int repetions = 0;

                    while (sound->IsPlaying() && repetions < 3)
                    {
                        SleepFor();
                        PrintTimeline(*sound);

                        if (sound->GetSeek() > sound->TotalTime()/2)
                        {
                            if (checkRep)
                            {
                                ++repetions;
                                checkRep = false;
                            }
                        }
                        else
                            checkRep = true;
                    }
                }
                else
                {
                    while (sound->IsPlaying())
                    {
                        SleepFor();
                        PrintTimeline(*sound);
                    }
                }

                std::cout << std::endl;
            }
            else
                std::cerr << "failed to load sound: \"" << soundFile << '\"' << std::endl;

            //to be continued ...
            
            // Break out from loop, we only need one single audio system
            break;
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
