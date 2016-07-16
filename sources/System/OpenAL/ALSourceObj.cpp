/*
 * ALSourceObj.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSourceObj.h"


namespace Ac
{


ALSourceObj::ALSourceObj()
{
    alGenSources(1, &handle_);
    if (alGetError() != AL_NO_ERROR)
        throw std::runtime_error("failed to generate OpenAL source object");
}

ALSourceObj::~ALSourceObj()
{
    if (handle_ != 0 && alIsSource(handle_))
        alDeleteSources(1, &handle_);
}

void ALSourceObj::AttachBuffer(const ALBufferObj& bufferObj)
{
    alSourcei(handle_, AL_BUFFER, bufferObj.Get());
}

void ALSourceObj::DetachBuffer()
{
    alSourcei(handle_, AL_BUFFER, AL_NONE);
}

ALint ALSourceObj::GetState() const
{
    ALint result = 0;
    alGetSourcei(handle_, AL_SOURCE_STATE, &result);
    return result;
}


} // /namespace Ac



// ================================================================================
