/*
 * ALSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSound.h"


namespace Ac
{


ALSound::ALSound()
{
}

ALSound::~ALSound()
{
}

void ALSound::Play()
{
    #if 1
    alSourcef(sourceObj_.Get(), AL_PITCH, 1.0f);
    alSourcef(sourceObj_.Get(), AL_GAIN, 1.0f);
    alSource3f(sourceObj_.Get(), AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(sourceObj_.Get(), AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(sourceObj_.Get(), AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(sourceObj_.Get(), AL_ROLLOFF_FACTOR, 0.5f);

    float nullOrientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, nullOrientation);
    #endif

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

void ALSound::SetBalance(float balance)
{
    //todo...
}

float ALSound::GetBalance() const
{
    return 0.0f;
}

void ALSound::SetFrequencyRatio(float ratio)
{
    alSourcef(sourceObj_.Get(), AL_PITCH, ratio);
}

float ALSound::GetFrequencyRatio() const
{
    float ratio = 0.0f;
    alGetSourcef(sourceObj_.Get(), AL_PITCH, &ratio);
    return ratio;
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
    //todo...
}

double ALSound::GetSeek() const
{
    //todo...
    return 0.0;
}

double ALSound::TotalTime() const
{
    //todo...
    return 0.0;
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
