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
        std::ifstream file("in/gong.ogg", std::ios_base::binary);
        if (file.good())
        {
            auto stream = audioSystem->OpenAudioStream(Ac::AudioStreamFormats::OGG, file);

            if (stream)
            {
                std::cout << "total stream time: " << stream->TotalTime() << "s" << std::endl;
                std::cout << "info comments:" << std::endl;
                for (const auto& s : stream->InfoComments())
                    std::cout << "  " << s << std::endl;

                std::cout << "streaming:" << std::endl;

                Ac::WaveBuffer buffer;
                buffer.SetSampleCount(10000);

                while (auto bytes = stream->StreamWaveBuffer(buffer))
                {
                    std::cout << "  " << bytes << " bytes read" << std::endl;
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
