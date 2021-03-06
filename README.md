AcousticsLib
============

This project provides a simple and cross-platform audio library for modern C++11.


Overview
--------

- **Version**: 1.00 Alpha
- **License**: [3-Clause BSD License](https://github.com/LukasBanana/AcousticsLib/blob/master/LICENSE.txt)
- **Documentation**: [AcousticsLib Docu.pdf](https://github.com/LukasBanana/AcousticsLib/blob/master/docu/AcousticsLib%20Docu.pdf)


Dependencies
------------

Using this library requires the [GaussianLib](https://github.com/LukasBanana/GaussianLib)

Building this library requires the [OpenAL SDK](http://openal.org/), and optionally the [Ogg-Vorbis](http://www.vorbis.com/) libraries


Supported Platforms
-------------------

| Platform | OpenAL | XAudio2 |
|----------|:------:|:-------:|
| **Windows Vista/ 7/ 8/ 10** | :heavy_check_mark: | :heavy_check_mark: |
| **Mac OS X** | :heavy_check_mark: | N/A |
| **Linux** | :heavy_check_mark: | N/A |


Supported File Formats
----------------------

| Format | File Extensions | Read Support | Write Support |
|--------|---------|:------------:|:-------------:|
| Waveform Audio File Format | **WAV** | :heavy_check_mark: | :heavy_check_mark: |
| Audio Interchange File Format | **AIFF**, **AIFC** | :heavy_check_mark: | :heavy_multiplication_x: |
| Ogg-Vorbis | **OGG** | :heavy_check_mark: | :heavy_multiplication_x: |


Audio Engines
-------------

- **OpenAL** *(in progress)*


Getting Started
---------------

Play a sound with less code as possible:

```cpp
#include <Ac/AcLib.h>

int main()
{
    // Load default audio system module
    auto audioSystem = Ac::AudioSystem::Load();

    // Play sound with 100% volume
    audioSystem->Play("mySound.wav");

    // Wait for user input
    int i = 0;
    std::cin >> i;

    return 0;
}
```

Play a sound by choosing a module:

```cpp
#include <Ac/AcLib.h>
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    // If an audio system is not available, dynamically catch the
    // exception an load another audio system, e.g. when "XAudio2"
    // is not available due to a missing DirectX version on a Windows platform.
    for (auto module : Ac::AudioSystem::FindModules())
    {
        try
        {
            // Load audio system from shared library
            auto audioSystem = Ac::AudioSystem::Load(module);

            std::cout
                << "Audio System: \" << audioSystem->GetName() << "\""
                << ", Version: " << audioSystem->GetVersion() <<
            std::endl;

            if (auto sound = audioSystem->LoadSound("mySound.wav"))
            {
                sound->SetVolume(0.5f); // set volume to 50%
                sound->SetPitch(1.5f);  // play with 1.5x frequency (or speed)
                sound->Play();          // play sound

                while (sound->IsPlaying())
                {
                    // Wait while sound is playing and show current seek position (in seconds)
                    std::cout << "Playing: " << sound->GetSeek() << std::end;

                    // Wait a moment
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

            // Break out from loop, we only need one single audio system
            break;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}
```


