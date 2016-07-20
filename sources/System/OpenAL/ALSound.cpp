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
    sourceObj_.SetInt(AL_SOURCE_RELATIVE, AL_TRUE);
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
    sourceObj_.SetFloat(AL_GAIN, volume);
}

float ALSound::GetVolume() const
{
    return sourceObj_.GetFloat(AL_GAIN);
}

void ALSound::SetPitch(float pitch)
{
    sourceObj_.SetFloat(AL_PITCH, pitch);
}

float ALSound::GetPitch() const
{
    return sourceObj_.GetFloat(AL_PITCH);
}

bool ALSound::IsPlaying() const
{
    return (sourceObj_.GetInt(AL_SOURCE_STATE) == AL_PLAYING);
}

bool ALSound::IsPaused() const
{
    return (sourceObj_.GetInt(AL_SOURCE_STATE) == AL_PAUSED);
}

void ALSound::SetSeek(double position)
{
    sourceObj_.SetFloat(AL_SEC_OFFSET, static_cast<float>(position));
}

double ALSound::GetSeek() const
{
    auto seek = static_cast<double>(sourceObj_.GetFloat(AL_SEC_OFFSET));
    if (bufferObjQueue_)
        seek += bufferObjQueue_->ProcessedTime();
    return seek;
}

double ALSound::TotalTime() const
{
    //TODO -> for streaming!!!
    return (bufferObj_ ? bufferObj_->GetTotalTime() : 0.0);
}

void ALSound::AttachBuffer(const WaveBuffer& waveBuffer)
{
    bufferObj_ = std::make_shared<ALBufferObj>(waveBuffer);
    sourceObj_.AttachBuffer(*bufferObj_);
}

void ALSound::QueueBuffer(const WaveBuffer& waveBuffer)
{
    /* Reset possible single buffer object */
    if (bufferObj_)
    {
        sourceObj_.DetachBuffer();
        bufferObj_.reset();
    }

    /* Initialize buffer object queue (if not done yet) */
    if (!bufferObjQueue_)
        bufferObjQueue_ = std::unique_ptr<ALBufferObjQueue>(new ALBufferObjQueue(sourceObj_.Get()));

    /* Fill buffer data */
    bufferObjQueue_->QueueBufferData(waveBuffer);
}

std::size_t ALSound::GetQueueSize() const
{
    return static_cast<std::size_t>(sourceObj_.GetInt(AL_BUFFERS_QUEUED));
}

std::size_t ALSound::GetProcessedQueueSize() const
{
    return static_cast<std::size_t>(sourceObj_.GetInt(AL_BUFFERS_PROCESSED));
}


} // /namespace Ac



// ================================================================================
