/*
 * test4_stream.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


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

        std::ifstream file(filename, std::ios_base::binary);

        auto sound = audioSystem->CreateSound();

        if (file.good())
        {
            auto stream = audioSystem->OpenAudioStream(
                (fileExt == ".ogg" ? Ac::AudioStreamFormats::OGG : Ac::AudioStreamFormats::MOD),
                file
            );

            if (stream)
            {
                std::cout << "total stream time: " << stream->TotalTime() << "s" << std::endl;
                std::cout << "info comments:" << std::endl;
                for (const auto& s : stream->InfoComments())
                    std::cout << "  " << s << std::endl;

                std::cout << "streaming:" << std::endl;

                Ac::WaveBuffer buffer;
                buffer.SetTotalTime(0.2);

                std::size_t bytesRead = 0, blocks = 0, bytes = 0;

                // Initialize buffer queue with 10 buffers
                for (int i = 0; i < 10; ++i)
                {
                    ++blocks;
                    bytes = stream->StreamWaveBuffer(buffer);
                    if (bytes > 0)
                    {
                        sound->QueueBuffer(buffer);
                        bytesRead += bytes;
                    }
                    else
                        break;
                }

                sound->SetVolume(0.5f);
                sound->Play();

                // Start continous streaming
                while (sound->IsPlaying())
                {
                    // Process next buffer
                    while (sound->GetProcessedQueueSize() > 0 && bytes > 0)
                    {
                        ++blocks;
                        bytes = stream->StreamWaveBuffer(buffer);

                        if (bytes > 0)
                        {
                            bytesRead += bytes;
                            sound->QueueBuffer(buffer);
                        }
                    }

                    std::cout << "  " << GetTimeline(sound->GetSeek(), stream->TotalTime(), 30) << "  " << blocks << " blocks and " << bytesRead << " bytes read\r";
                    std::flush(std::cout);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
