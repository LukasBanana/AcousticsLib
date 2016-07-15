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

            std::cout << "Audio System: \"" << audioSystem->GetName() << "\"" << std::endl;

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
