/*
 * ALAudioSystem.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALAudioSystem.h"
#include "../../FileHandler/WAVReader.h"
#include "ALSound.h"

#include <fstream>


namespace Ac
{


ALAudioSystem::ALAudioSystem() :
    device_ ( OpenDevice()    ),
    context_( CreateContext() )
{
}

ALAudioSystem::~ALAudioSystem()
{
    /* Clean up all OpenAL resources */
    alcMakeContextCurrent(nullptr);
    if (context_)
        alcDestroyContext(context_);
    if (device_)
        alcCloseDevice(device_);
}

std::string ALAudioSystem::GetVersion() const
{
    auto version = alGetString(AL_VERSION);
    auto renderer = alGetString(AL_RENDERER);
    auto vendor = alGetString(AL_VENDOR);

    std::string s = "OpenAL";
    if (version)
        s += " " + std::string(version);
    if (renderer)
        s += " (" + std::string(renderer) + ")";
    if (vendor)
        s += " " + std::string(vendor);

    return s;
}

std::unique_ptr<Sound> ALAudioSystem::LoadSound(const std::string& filename)
{
    auto bufferObj = CreateBufferObjFromFile(filename);
    if (bufferObj)
    {
        auto sound = std::unique_ptr<ALSound>(new ALSound());
        sound->AttachBuffer(std::move(bufferObj));
        return std::move(sound);
    }
    return nullptr;
}


/*
 * ======= Private: =======
 */

std::vector<std::string> ALAudioSystem::ListDevices() const
{
    std::vector<std::string> deviceNames;

    auto device = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    auto next   = device + 1;

    while (device && *device != '\0' && next && *next != '\0')
    {
        deviceNames.push_back(device);

        auto len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }

    return deviceNames;
}

ALCdevice* ALAudioSystem::OpenDevice()
{
    auto device = alcOpenDevice(nullptr);
    if (!device)
        throw std::runtime_error("failed to open OpenAL device");
    return device;
}

ALCcontext* ALAudioSystem::CreateContext()
{
    auto context = alcCreateContext(device_, nullptr);
    if (!context)
        throw std::runtime_error("failed to create OpenAL context");
    if (!alcMakeContextCurrent(context))
        throw std::runtime_error("failed to make OpenAL context current");
    return context;
}

std::unique_ptr<ALBufferObj> ALAudioSystem::CreateBufferObjFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.good())
    {
        /* Read sound file */
        WAVReader reader;

        WaveBuffer waveBuffer;
        reader.ReadWaveBuffer(file, waveBuffer);

        /* Create AL buffer object and fill with wave buffer data */
        auto bufferObj = std::unique_ptr<ALBufferObj>(new ALBufferObj());
        bufferObj->BufferData(waveBuffer);

        return bufferObj;
    }

    return nullptr;
}


} // /namespace Ac



// ================================================================================
