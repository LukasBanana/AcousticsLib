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
#include <cstring>


namespace Ac
{


/* ----- Audio system ----- */

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

/* ----- Sounds ----- */

std::unique_ptr<Sound> ALAudioSystem::CreateSound()
{
    return std::unique_ptr<ALSound>(new ALSound());
}

/* ----- Listener ----- */

void ALAudioSystem::SetListenerPosition(const Gs::Vector3f& position)
{
    alListenerfv(AL_POSITION, position.Ptr());
}

Gs::Vector3f ALAudioSystem::GetListenerPosition() const
{
    Gs::Vector3f position;
    alGetListenerfv(AL_POSITION, position.Ptr());
    return position;
}

void ALAudioSystem::SetListenerVelocity(const Gs::Vector3f& velocity)
{
    alListenerfv(AL_POSITION, velocity.Ptr());
}

Gs::Vector3f ALAudioSystem::GetListenerVelocity() const
{
    Gs::Vector3f velocity;
    alGetListenerfv(AL_VELOCITY, velocity.Ptr());
    return velocity;
}

void ALAudioSystem::SetListenerOrientation(const ListenerOrientation& orientation)
{
    const ALfloat vec[6] =
    {
        orientation.atVector.x, orientation.atVector.y, -orientation.atVector.z,
        orientation.upVector.x, orientation.upVector.y, -orientation.upVector.z
    };
    alListenerfv(AL_ORIENTATION, vec);
}

ListenerOrientation ALAudioSystem::GetListenerOrientation() const
{
    ALfloat vec[6] = { 0 };
    alGetListenerfv(AL_ORIENTATION, vec);

    ListenerOrientation orientation;

    orientation.atVector.x =  vec[0];
    orientation.atVector.y =  vec[1];
    orientation.atVector.z = -vec[2];

    orientation.upVector.x =  vec[3];
    orientation.upVector.y =  vec[4];
    orientation.upVector.z = -vec[5];

    return orientation;
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


} // /namespace Ac



// ================================================================================
