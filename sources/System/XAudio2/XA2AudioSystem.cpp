/*
 * XA2AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2AudioSystem.h"
#include "XA2Sound.h"


namespace Ac
{


XA2AudioSystem::XA2AudioSystem()
{
    #ifndef _XBOX
    /* Initialize COM (Component Object Model) system */
    if (CoInitializeEx(nullptr, COINIT_MULTITHREADED) != S_OK)
        throw std::runtime_error("failed to initialize COM interface for XAudio2");
    #endif

    /* Create sound device and mastering voice */
    if (XAudio2Create(device_.ReleaseAndGetAddressOf()) != S_OK)
        throw std::runtime_error("failed to create XAudio2 device");

    if (device_->CreateMasteringVoice(&masteringVoice_) != S_OK)
        throw std::runtime_error("failed to create XAudio2 mastering voice");
}

XA2AudioSystem::~XA2AudioSystem()
{
    if (masteringVoice_)
        masteringVoice_->DestroyVoice();

    #ifndef _XBOX
    CoUninitialize();
    #endif
}

/* ----- Audio system ----- */

std::string XA2AudioSystem::GetVersion() const
{


    return "XAudio2";
}

/* ----- Sounds ----- */

std::unique_ptr<Sound> XA2AudioSystem::CreateSound()
{
    return std::unique_ptr<Sound>(new XA2Sound(device_.Get()));
}

/* ----- Listener ----- */

void XA2AudioSystem::SetListenerPosition(const Gs::Vector3f& position)
{
}

Gs::Vector3f XA2AudioSystem::GetListenerPosition() const
{
    Gs::Vector3f position;
    return position;
}

void XA2AudioSystem::SetListenerVelocity(const Gs::Vector3f& velocity)
{
}

Gs::Vector3f XA2AudioSystem::GetListenerVelocity() const
{
    Gs::Vector3f velocity;
    return velocity;
}

void XA2AudioSystem::SetListenerOrientation(const ListenerOrientation& orientation)
{
}

ListenerOrientation XA2AudioSystem::GetListenerOrientation() const
{
    ListenerOrientation orientation;
    return orientation;
}


} // /namespace Ac



// ================================================================================
