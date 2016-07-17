/*
 * test4_stream.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <thread>
#include <fstream>


int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load("OpenAL");

        // Open OggVorbis stream
        std::ifstream file("gong.ogg");
        if (file.good())
        {
            auto stream = audioSystem->OpenAudioStream(Ac::AudioStreamFormats::OGG, file);

            if (stream)
            {
                std::cout << "Total Stream Time: " << stream->TotalTime() << "s" << std::endl;

                //while ( ( auto p =  )  )
                {

                }
            }
        
            std::cout << "finished" << std::endl;
        }
        else
            std::cerr << "failed to read Ogg Vorbis file" << std::endl;
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
