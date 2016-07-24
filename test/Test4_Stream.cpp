/*
 * Test4_Stream.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


//#define TEST_ENABLE_MANUAL_STREAMING

int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load("OpenAL");

        // Open audio stream
        const std::string filename = (
            "in/gong.ogg"
            //"in/chiptune_2.mod"
        );

        const std::string fileExt = filename.substr(filename.size() - 4);

        auto sound = audioSystem->LoadSound(filename);

        if (sound)
        {
            auto stream = sound->GetStreamSource();

            if (stream)
            {
                std::cout << "total stream time: " << stream->TotalTime() << "s" << std::endl;
                std::cout << "info comments:" << std::endl;
                for (const auto& s : stream->InfoComments())
                    std::cout << "  " << s << std::endl;

                std::cout << "streaming:" << std::endl;

                Ac::WaveBuffer buffer;
                buffer.SetTotalTime(0.2);

                #ifdef TEST_ENABLE_MANUAL_STREAMING

                std::size_t bytesRead = 0, blocks = 0, bytes = 1;

                #endif

                sound->SetVolume(0.5f);
                sound->Play();

                // Start continous streaming
                while (sound->IsPlaying())
                {
                    #ifdef TEST_ENABLE_MANUAL_STREAMING

                    // Process next buffer
                    while (sound->GetProcessedQueueSize() > 0)
                    {
                        ++blocks;
                        bytes = stream->StreamWaveBuffer(buffer);

                        if (bytes > 0)
                        {
                            bytesRead += bytes;
                            sound->QueueBuffer(buffer);
                        }
                        else
                            break;
                    }
                    
                    SleepFor();
                    
                    if (sound->IsPlaying())
                    {
                        std::cout << "\r  " << GetTimeline(*sound, 30) << "  " << blocks << " blocks and " << bytesRead << " bytes read";
                        std::flush(std::cout);
                    }

                    #else

                    // Process audio streaming
                    audioSystem->Streaming(*sound);
                    
                    SleepFor();
                    
                    if (sound->IsPlaying())
                        PrintTimeline(*sound);

                    #endif
                }

                std::cout << std::endl;
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
