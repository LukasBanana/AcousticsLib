/*
 * test1.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <string>


int main()
{
    try
    {
        // Create audio system
        auto audioSystem = Ac::AudioSystem::Load("OpenAL");
        
        audioSystem = Ac::AudioSystem::Load("XAudio2");


        std::cout << "Audio System: \"" << audioSystem->GetName() << "\"" << std::endl;

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
