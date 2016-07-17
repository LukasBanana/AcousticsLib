/*
 * ALSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSound.h"

#include <cmath>
#include <algorithm>


namespace Ac
{


ALSound::ALSound()
{
    /* Use relative coordinates to make a appear as a 2D sound */
    alSourcei(sourceObj_.Get(), AL_SOURCE_RELATIVE, AL_TRUE);
}

ALSound::~ALSound()
{
}

void ALSound::Play()
{
    alSourcePlay(sourceObj_.Get());
}

void ALSound::Pause()
{
    alSourcePause(sourceObj_.Get());
}

void ALSound::Stop()
{
    alSourceStop(sourceObj_.Get());
}

void ALSound::SetVolume(float volume)
{
    alSourcef(sourceObj_.Get(), AL_GAIN, volume);
}

float ALSound::GetVolume() const
{
    float volume = 0.0f;
    alGetSourcef(sourceObj_.Get(), AL_GAIN, &volume);
    return volume;
}

void ALSound::SetPitch(float pitch)
{
    alSourcef(sourceObj_.Get(), AL_PITCH, pitch);
}

float ALSound::GetPitch() const
{
    float pitch = 0.0f;
    alGetSourcef(sourceObj_.Get(), AL_PITCH, &pitch);
    return pitch;
}

bool ALSound::IsPlaying() const
{
    return (sourceObj_.GetState() == AL_PLAYING);
}

bool ALSound::IsPaused() const
{
    return (sourceObj_.GetState() == AL_PAUSED);
}

void ALSound::SetSeek(double position)
{
    alSourcef(sourceObj_.Get(), AL_SEC_OFFSET, static_cast<float>(position));
}

double ALSound::GetSeek() const
{
    float position = 0.0f;
    alGetSourcef(sourceObj_.Get(), AL_SEC_OFFSET, &position);
    return static_cast<double>(position);
}

double ALSound::TotalTime() const
{
    return (bufferObj_ ? bufferObj_->GetTotalTime() : 0.0);
}

void ALSound::AttachBuffer(const std::shared_ptr<ALBufferObj>& bufferObj)
{
    bufferObj_ = bufferObj;
    if (bufferObj)
        sourceObj_.AttachBuffer(*bufferObj);
    else
        sourceObj_.DetachBuffer();
}


} // /namespace Ac



// ================================================================================
