/*
 * ALAudioSystem.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALAudioSystem.h"

#include <iostream>//!!!


namespace Ac
{


ALAudioSystem::ALAudioSystem() :
    device_ ( OpenDevice()    ),
    context_( CreateContext() )
{
}

ALAudioSystem::~ALAudioSystem()
{
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


/*
 * ======= Private: =======
 */

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


} // /namespace Ac



// ================================================================================
