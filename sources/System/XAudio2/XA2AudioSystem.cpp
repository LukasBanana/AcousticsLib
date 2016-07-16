/*
 * XA2AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2AudioSystem.h"


namespace Ac
{


std::string XA2AudioSystem::GetVersion() const
{
    return "XAudio2";
}

std::unique_ptr<Sound> XA2AudioSystem::LoadSound(const std::string& filename)
{
    return nullptr; // todo...
}


} // /namespace Ac



// ================================================================================
