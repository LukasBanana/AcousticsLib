/*
 * ALSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSound.h"
#include "../../Core/Streaming.h"

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

/* ----- Playback ----- */

void ALSound::Play()
{
    /* Initialize streaming */
    if (GetStreamSource() && !IsPaused())
        InitStreaming(*this);

    /* Play sound source */
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

void ALSound::SetLooping(bool enable)
{
    if (looping_ != enable)
    {
        looping_ = enable;
        if (!GetStreamSource())
            sourceObj_.SetInt(AL_LOOPING, (looping_ ? AL_TRUE : AL_FALSE));
    }
}

bool ALSound::GetLooping() const
{
    return looping_;
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
    if (GetStreamSource())
        return GetStreamSource()->TotalTime();
    if (bufferObj_)
        return bufferObj_->GetTotalTime();
    return 0.0;
}

/* ----- Buffers and streaming ----- */

void ALSound::AttachBuffer(const WaveBuffer& waveBuffer)
{
    /* Reset possible buffer queue object */
    ResetBufferObjQueue();

    /* Stop palyback and detach previous buffer object */
    Stop();
    sourceObj_.DetachBuffer();

    /* Create new buffer object or fill previous one */
    if (!bufferObj_)
        bufferObj_ = std::make_shared<ALBufferObj>(waveBuffer);
    else
        bufferObj_->BufferData(waveBuffer);

    /* Attach buffer object to source */
    sourceObj_.AttachBuffer(*bufferObj_);
}

void ALSound::AttachSharedBuffer(const Sound& sourceBufferSound)
{
    const auto& sourceBufferSoundAL = static_cast<const ALSound&>(sourceBufferSound);

    /* Stop palyback and detach previous buffer object */
    Stop();
    sourceObj_.DetachBuffer();

    /* Shared buffer object with the specified source buffer sound */
    bufferObj_ = sourceBufferSoundAL.bufferObj_;

    /* Attach buffer object to source */
    if (bufferObj_)
        sourceObj_.AttachBuffer(*bufferObj_);
}

void ALSound::QueueBuffer(const WaveBuffer& waveBuffer)
{
    /* Reset possible single buffer object */
    ResetBufferObj();

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

/* ----- 3D sound ----- */

void ALSound::Enable3D(bool enable)
{
    enabled3D_ = enable;
    sourceObj_.SetVector3(AL_POSITION, { 0.0f, 0.0f, 0.0f });
    sourceObj_.SetVector3(AL_VELOCITY, { 0.0f, 0.0f, 0.0f });
    sourceObj_.SetInt(AL_SOURCE_RELATIVE, (enable ? AL_FALSE : AL_TRUE));
}

bool ALSound::Is3DEnabled() const
{
    return enabled3D_;
}

void ALSound::SetPosition(const Gs::Vector3f& position)
{
    if (enabled3D_)
        sourceObj_.SetVector3(AL_POSITION, position);
}

Gs::Vector3f ALSound::GetPosition() const
{
    return sourceObj_.GetVector3(AL_POSITION);
}

void ALSound::SetVelocity(const Gs::Vector3f& velocity)
{
    if (enabled3D_)
        sourceObj_.SetVector3(AL_VELOCITY, velocity);
}

Gs::Vector3f ALSound::GetVelocity() const
{
    return sourceObj_.GetVector3(AL_VELOCITY);
}

void ALSound::SetSpaceRelative(bool enable)
{
    if (enabled3D_)
        sourceObj_.SetInt(AL_SOURCE_RELATIVE, (enable ? AL_TRUE : AL_FALSE));
}

bool ALSound::GetSpaceRelative() const
{
    return (sourceObj_.GetInt(AL_SOURCE_RELATIVE) != AL_FALSE);
}


/*
 * ======= Private: =======
 */

void ALSound::ResetBufferObj()
{
    if (bufferObj_)
    {
        /* Detach buffer object from source object */
        sourceObj_.DetachBuffer();
        bufferObj_.reset();

        /* Remove OpenAL looping state (only a single buffer can make use of this state) */
        if (looping_)
            sourceObj_.SetInt(AL_LOOPING, AL_FALSE);
    }
}

void ALSound::ResetBufferObjQueue()
{
    if (bufferObjQueue_)
    {
        /* Reset buffer object queue */
        bufferObjQueue_.reset();

        /* Reset OpenAL looping state (only a single buffer can make use of this state) */
        if (looping_)
            sourceObj_.SetInt(AL_LOOPING, AL_TRUE);
    }
}


} // /namespace Ac



// ================================================================================
