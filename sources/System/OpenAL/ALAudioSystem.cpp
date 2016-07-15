/*
 * ALAudioSystem.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALAudioSystem.h"


namespace Ac
{


ALAudioSystem::ALAudioSystem() :
    device_( OpenDevice() )
{
}

ALAudioSystem::~ALAudioSystem()
{
}


/*
 * ======= Private: =======
 */

ALCdevice* ALAudioSystem::OpenDevice()
{
    auto device = alcOpenDevice(nullptr);
    if (!device)
        throw std::runtime_error("failed to open OpenAL device");
}


} // /namespace Ac



// ================================================================================
