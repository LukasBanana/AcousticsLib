/*
 * Test2.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"


#define TEST_MODE_SYNTH     1
#define TEST_MODE_NOISE     2
#define TEST_MODE_LOAD      3

#define TEST_MODE           TEST_MODE_LOAD

#define TEST_WRITE_OUTPUT   1

int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load();
        
        #if 1

        Ac::WaveBuffer buffer(Ac::WaveBufferFormat(Ac::sampleRate44kHz, 16, 1)), outputBuffer;

        #if TEST_MODE == TEST_MODE_SYNTH

        buffer.SetTotalTime(4.0);
        //Ac::Synthesizer::GenerateSineWave(buffer, 0.0, 2.0, 0.8, 0.0, 300.0);
        //Ac::Synthesizer::GenerateSineWave(buffer, 1.0, 2.0, 0.2, 0.0, 1500.0);

        buffer.ForEachSample(
            [](double& sample, std::uint16_t channel, std::size_t index, double phase)
            {
                #if 0

                using N = Ac::MusicalNotes;
                std::array<N, 7> notes { N::C, N::D, N::E, N::F, N::G, N::A, N::B };
                
                auto phaseIdx = static_cast<size_t>(phase*5.0);
                auto interval = 4 + phaseIdx / notes.size();
                auto noteIdx = phaseIdx % notes.size();

                auto freq = Ac::Synthesizer::GetNoteFrequency(notes[noteIdx], interval);
                auto sineWaveGen = Ac::Synthesizer::SineWaveGenerator(0.3, 0.0, freq);

                sineWaveGen(sample, channel, phase);

                #elif 1

                using N = Ac::MusicalNotes;
                struct Note
                {
                    N   n;
                    int i;
                };
                std::array<Note, 9> notes
                {
                    Note{ N::E, 4 },
                    Note{ N::DSharp, 4 },
                    Note{ N::E, 4 },
                    Note{ N::DSharp, 4 },
                    Note{ N::E, 4 },
                    Note{ N::B, 3 },
                    Note{ N::D, 4 },
                    Note{ N::C, 4 },
                    Note{ N::A, 3 }
                };
                
                auto phaseIdx = static_cast<size_t>(phase*4.0);
                auto noteIdx = phaseIdx % notes.size();

                auto freq = Ac::Synthesizer::GetNoteFrequency(notes[noteIdx].n, notes[noteIdx].i);
                auto sineWaveGen = Ac::Synthesizer::SineGenerator(0.3, 0.0, freq);

                sineWaveGen(sample, channel, index, phase);

                #elif 1

                if (phase < 2.0)
                    Ac::Synthesizer::HalfCircleWaveGenerator(0.3, 0.0, 440.0)(sample, channel, index, phase);
                else
                    Ac::Synthesizer::SineWaveGenerator(0.3, 0.0, 440.0)(sample, channel, index, phase);

                #endif
            }
        );

        outputBuffer = buffer;

        //outputBuffer.Append(buffer);

        #elif TEST_MODE == TEST_MODE_NOISE

        outputBuffer.SetTotalTime(1.0);

        outputBuffer.ForEachSample(Ac::Synthesizer::NoiseGenerator(0.3));
        Ac::Synthesizer::BlurWaveBuffer(outputBuffer, 0.2, 1.0, 100);

        #elif TEST_MODE == TEST_MODE_LOAD

        std::string inputFilename = "in/thorndike.wav";
        std::cout << "read sound file: \"" << inputFilename << '\"' << std::endl;
        
        std::ifstream inputFile(inputFilename, std::ios_base::binary);
        buffer = audioSystem->ReadWaveBuffer(inputFile);

        outputBuffer = buffer;

        /*outputBuffer.ForEachSample(
            [&](double& sample, std::uint16_t channel, std::size_t index, double phase)
            {
                sample = buffer.ReadSample(phase, channel);
                if (phase >= 0.2)
                    sample += buffer.ReadSample(phase - 0.2, channel)*0.3;
                if (phase >= 0.4)
                    sample += buffer.ReadSample(phase - 0.4, channel)*0.15;
            }
        );*/

        //outputBuffer.SetFormat(Ac::WaveBufferFormat(Ac::Synthesizer::sampleRate8kHz, 8, 1));

        //Ac::Synthesizer::ReverseWaveBuffer(outputBuffer);

        Ac::Synthesizer::BlurWaveBuffer(outputBuffer, 0.1, 1.0, 15);
        outputBuffer.ForEachSample(Ac::Synthesizer::Amplifier(3.0));

        #endif

        #if TEST_WRITE_OUTPUT != 0
        
        std::string outputFilename = "out/synthesized_sound.wav";
        std::cout << "write sound file: \"" << outputFilename << '\"' << std::endl;
        
        std::ofstream outputFile(outputFilename, std::ios_base::binary);
        audioSystem->WriteAudioBuffer(Ac::AudioFormats::WAVE, outputFile, outputBuffer);

        #endif

        auto sound = audioSystem->CreateSound(outputBuffer);
    
        if (sound)
        {
            sound->SetVolume(0.25f);
            sound->Play();

            while (sound->IsPlaying())
            {
                PrintTimeline(*sound);
                SleepFor();
            }
            
            std::cout << std::endl;
        }
    
        #else
    
        audioSystem->PlaySound(
            "in/shutter.wav", 1.0f, 0,
            [](Ac::Sound& s)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            
                #if 1
            
                s.SetVolume(s.GetVolume() - 0.01f);
                std::cout << "playing: " << s.GetSeek() << " / " << s.TotalTime() << std::endl;
                
                #else

                static float a;
                a += 0.05f;
                auto balance = std::sin(a);
            
                s.SetPosition({ balance, 0.0f, -std::sqrt(1.0f - balance*balance) });
            
                std::cout << "balance: " << balance;
                if (balance < -0.5f)
                    std::cout << ", left";
                else if (balance > 0.5f)
                    std::cout << ", right";
                else
                    std::cout << ", center";
                std::cout << std::endl;
            
                #endif
            
                return s.GetVolume() > 0.05f;
            }
        );

        #endif
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
