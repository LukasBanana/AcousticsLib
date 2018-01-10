/*
 * NullAudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NullAudioSystem.h"
#include "NullSound.h"


namespace Ac
{


/* ----- Audio system ----- */

std::string NullAudioSystem::GetVersion() const
{
    return "Null";
}

/* ----- Sounds ----- */

std::unique_ptr<Sound> NullAudioSystem::CreateSound()
{
    return std::unique_ptr<Sound>(new NullSound());
}

/* ----- Listener ----- */

void NullAudioSystem::SetListenerPosition(const Gs::Vector3f& position)
{
    listenerPosition_ = position;
}

Gs::Vector3f NullAudioSystem::GetListenerPosition() const
{
    return listenerPosition_;
}

void NullAudioSystem::SetListenerVelocity(const Gs::Vector3f& velocity)
{
    listenerVelocity_ = velocity;
}

Gs::Vector3f NullAudioSystem::GetListenerVelocity() const
{
    return listenerVelocity_;
}

void NullAudioSystem::SetListenerOrientation(const ListenerOrientation& orientation)
{
    listenerOrientation_ = orientation;
}

ListenerOrientation NullAudioSystem::GetListenerOrientation() const
{
    return listenerOrientation_;
}


} // /namespace Ac



// ================================================================================
