/*
 * AcLib.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_INCLUDE_H__
#define __AC_INCLUDE_H__


#include "AudioSystem.h"
#include "WaveBuffer.h"
#include "Synthesizer.h"
#include "ChannelTypes.h"
#include "Visualizer.h"


/**
\mainpage AcousticsLib 1.00 Alpha Documentation

\section sec_table Table of Contents

- \ref sec_intro
- \ref sec_getstarted
- \ref sec_example1
- \ref sec_example2
- \ref sec_example3
- \ref sec_example4
- \ref sec_example5

\section sec_intro Introduction

Welcome to AcousticsLib, a cross-platform and open-source C++ library for real-time audio processing.

The following platforms are supported:
- <b>Windows Vista/ 7/ 8/ 10</b>
- <b>Mac OS X</b>
- <b>Linux</b>

The following audio file formats are supported:
- <b>WAV</b> (<i>Waveform Audio File Format</i>) Read/Write
- <b>AIFF</b> (<i>Audio Interchange File Format</i>) Read
- <b>OGG</b> (<i>Ogg-Vorbis</i>) Read

The following audio engines are provided:
- <b>OpenAL</b>

\section sec_getstarted Getting Started

To start using AcousticsLib in your C++ project you need the following prerequisites:
- C++11 compliant compiler (i.e. <b>Visual C++ 2015</b> or later, <b>G++</b> or <b>clang</b> with <tt>-std=c++11</tt> option available)
- <a href="https://github.com/LukasBanana/GaussianLib">GaussianLib</a> header files

To build the AcousticsLib you will also need the following prerequisites:
- <a href="https://cmake.org/">CMake</a> 2.8 or later to build the project files (e.g. for VisualStudio, Xcode, Code::Blocks etc.)
- <a href="https://www.openal.org/">OpenAL</a> SDK
- <a href="https://www.xiph.org/downloads/">Ogg Vorbis</a> source files of <b>libogg</b> and <b>libvorbis</b>

After setting up all prerequisites, set the <b>include search path</b> to <tt><AcousticsLib-Path>/include</tt>
and add the library file <tt>AcLib</tt> (<tt>AcLib.lib</tt> for Visual C++ and <tt>libAcLib.a</tt> for G++ and clang) to the dependencies in your project.

\section sec_example1 Example 1: Play a sound

This is an example of playing a sound in the simplest way:
\code
#include <Ac/AcLib.h>
#include <iostream>

int main()
{
    // Load audio system and play a sound from file.
    auto audioSystem = Ac::AudioSystem::Load();
    audioSystem->Play("MySound.wav");

    // Wait for user input before quit
    int i = 0;
    std::cin >> i;

    return 0;
}
\endcode

\section sec_example2 Example 2: Read wave buffers

This is an example about reading wave buffers and creating sounds manually:
\code
#include <Ac/AcLib.h>
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    try
    {
        // Load specific audio system. To find all available system,
        // use "Ac::AudioSystem::FindModules()". This must be an std::shared_ptr,
        // because the audio system keeps track of this reference with an std::weak_ptr.
        // If a new audio system is loaded, all references must be reset,
        // since only a single audio system can be loaded at a time.
        std::shared_ptr<Ac::AudioSystem> audioSystem = Ac::AudioSystem::Load("OpenAL");

        // Read wave buffer from file. The return type is "Ac::WaveBuffer".
        Ac::WaveBuffer waveBuffer = audioSystem->ReadWaveBuffer("MySound.wav");

        // Add some noise to the wave buffer with an amplitude of 0.1.
        // The "NoiseGenerator" function will return a function object,
        // which is applied to each sample.
        waveBuffer.ForEeachSample(Ac::Synthesizer::NoiseGenerator(0.1));

        // Blur the buffer to make is sound like a robot voice.
        // Time spread is 0.2, gaussian curve variance is 1.0 (standard deviation),
        // and use 100 samples to compute the blur.
        Ac::Synthesizer::BlurWaveBuffer(waveBuffer, 0.2, 1.0, 100);

        // Create a sound with our wave buffer.
        std::unique_ptr<Ac::Sound> sound = audioSystem->CreateSound(waveBuffer);

        // Play the sound with a pitch (or frequency factor) of 70% and volume of 80%.
        sound->SetPitch(0.7f);
        sound->SetVolume(0.8f);
        sound->Play();

        // Wait while the sound is playing
        while (sound->IsPlaying())
        {
            // Sleep for 100 milliseconds to let other processes run
            // (playing the sound is done in the background).
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception& e)
    {
        // Print out exception message if something went wrong
        // (e.g. sound file is corrupted or the like).
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
\endcode

\section sec_example3 Example 3: Music streaming

This is a small example about music streaming:
\code
#include <Ac/AcLib.h>

int main()
{
    auto audioSystem = Ac::AudioSystem::Load();

    // Load and play Ogg-Vorbis music stream from file.
    auto sound = audioSystem->LoadSound("MyMusic.ogg");
    sound->Play();

    // Process music streaming
    // (this must currently be done manually with the "Streaming" function).
    while (sound->IsPlaying())
    {
        // Process next streaming block. This function automatically
        // checks if new streaming buffers can be queued.
        audioSystem->Streaming(*sound);
    }

    return 0;
}
\endcode

\section sec_example4 Example 4: 3D Sound

This is a small example about 3D sounds:
\code
#include <Ac/AcLib.h>
#include <thread>
#include <chrono>

int main()
{
    auto audioSystem = Ac::AudioSystem::Load();

    // Load sound from file and enable 3D features.
    auto sound = audioSystem->LoadSound("MySound.wav", Ac::SoundFlags::Enable3D);

    // Setup 3D position in the left-handed coordinate system.
    sound->SetPosition({ 5, 0, 2 });
    sound->Play();

    while (sound->IsPlaying())
    {
        // Move sound in 3D space.
        Gs::Vector3 deltaPos(-0.1f, 0, 0);
        sound->SetPosition(sound->GetPosition() + deltaPos);

        // Set sound velocity for the doppler effect.
        sound->SetVelocity(deltaPos);

        // Wait for a moment
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
\endcode

\section sec_example5 Example 5: Recording

This is an example about recording with a microphone device (currently only supported on the <i>Windows</i> platform):
\code
#include <Ac/AcLib.h>
#include <chrono>

int main()
{
    try
    {
        auto audioSystem = Ac::AudioSystem::Load();

        // Query a microphone object and use standard device.
        std::unique_ptr<Ac::Microphone> mic = audioSystem->QueryMicrohpone();

        if (mic)
        {
            // Start recording process with the following wave buffer format:
            // 44.1 kHz sample rate, 16 bits per sample, 1 channel, and sample every 0.1 seconds.
            mic->Start(Ac::WaveBufferFormat(Ac::sampleRate44kHz, 16, 1), 0.1);

            // Create an empty sound to play immediately what we've recorded.
            auto sound = audioSystem->CreateSound();

            // Record for a specific amount of time.
            auto startTime = std::chrono::system_clock::now();

            while (mic->IsRecording())
            {
                // Receiver wave buffer from microphone.
                std::unique_ptr<Ac::WaveBuffer> buffer = mic->ReceivedInput();
                if (buffer)
                {
                    // Print some information about the received buffer.
                    std::cout
                        << "Received Buffer: Sample Frames = " << buffer->GetSampleFrames() << ", "
                        << "Duration = " << buffer->GetTotalTime() << "s, "
                        << "Queue Size = " << sound->GetQueueSize() << std::endl;
                    
                    // Modify the input buffer before adding it to the soud queue.
                    Ac::Synthesizer::BlurWaveBuffer(*buffer, 0.1, 1.0, 15);

                    // Queue the new buffer to our sound.
                    sound->QueueBuffer(*buffer);

                    // Start playing the sound when we have enough buffers in the queue.
                    if (sound->GetQueueSize() == 2)
                        sound->Play();
                }

                // Check if time for recording test is over (5 seconds)
                auto now = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count() > 5)
                {
                    // Stop recording process.
                    mic->Stop();
                }
            }
        }
        else
            std::cerr << "No microphone device available" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
\endcode

*/


#endif



// ================================================================================
