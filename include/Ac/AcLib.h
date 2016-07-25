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
    // Load audio system and play a sound from file
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

int main()
{
    try
    {
        // Load specific audio system. To find all available system, use "Ac::AudioSystem::FindModules()"
        auto audioSystem = Ac::AudioSystem::Load("OpenAL");

        // Read wave buffer from file. The return type is "Ac::WaveBuffer".
        auto waveBuffer = audioSystem->ReadWaveBuffer("MySound.wav");

        // Modify the wave buffer 

        
    }
    catch (const std::exception& e)
    {
        // Print out exception message if something went wrong (e.g. sound file is corrupted or the like)
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
\endcode

*/


#endif



// ================================================================================
