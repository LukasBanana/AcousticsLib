/*
 * ALSourceObj.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSourceObj.h"
#include "ALFormat.h"


namespace Ac
{


ALSourceObj::ALSourceObj()
{
    alGenSources(1, &handle_);

    if (handle_ == 0)
    {
        auto err = alGetError();
        if (err != AL_NO_ERROR)
            throw std::runtime_error("failed to generate OpenAL source object (error code: " + ALErrorToString(err) + ")");
        else
            throw std::runtime_error("failed to generate OpenAL source object (error code: <unknown>)");
    }
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

void ALSourceObj::SetInt(ALenum param, ALint value)
{
    alSourcei(handle_, param, value);
}

ALint ALSourceObj::GetInt(ALenum param) const
{
    ALint value = 0;
    alGetSourcei(handle_, param, &value);
    return value;
}

void ALSourceObj::SetFloat(ALenum param, ALfloat value)
{
    alSourcef(handle_, param, value);
}

ALfloat ALSourceObj::GetFloat(ALenum param) const
{
    ALfloat value = 0.0f;
    alGetSourcef(handle_, param, &value);
    return value;
}

void ALSourceObj::SetVector3(ALenum param, const Gs::Vector3f value)
{
    /* Set vector and swap Z coordinate from a right-handed to a left-handed coordinate system */
    alSource3f(handle_, param, value.x, value.y, -value.z);
}

Gs::Vector3f ALSourceObj::GetVector3(ALenum param) const
{
    /* Get vector and swap Z coordinate from a right-handed to a left-handed coordinate system */
    Gs::Vector3f value(Gs::UninitializeTag{});
    alGetSource3f(handle_, param, &value.x, &value.y, &value.z);
    value.z = -value.z;
    return value;
}


} // /namespace Ac



// ================================================================================
