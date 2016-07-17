/*
 * XA2AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2AudioSystem.h"


namespace Ac
{


/* ----- Audio system ----- */

std::string XA2AudioSystem::GetVersion() const
{
    return "XAudio2";
}

/* ----- Sounds ----- */

std::unique_ptr<Sound> XA2AudioSystem::LoadSound(const std::string& filename)
{
    return nullptr; // todo...
}

std::unique_ptr<Sound3D> XA2AudioSystem::LoadSound3D(const std::string& filename)
{
    return nullptr; // todo...
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
