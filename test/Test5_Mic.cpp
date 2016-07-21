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
