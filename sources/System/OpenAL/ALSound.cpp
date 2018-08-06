/*
 * ALSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSound.h"
#include "ALSourceObjManager.h"
#include "../../Core/Streaming.h"

#include <cmath>
#include <algorithm>


namespace Ac
{


static ALint ALBool(bool value)
{
    return (value ? AL_TRUE : AL_FALSE);
}

ALSound::~ALSound()
{
    ALSourceObjManager::Instance().FreeSourceObj(sourceObj_);
}

/* ----- Playback ----- */

void ALSound::Play()
{
    /* Acquire source object if not done yet */
    if (AcquireSourceObj())
    {
        /* Initialize streaming */
        if (GetStreamSource() && !IsPaused())
            InitStreaming(*this);

        /* Play sound source */
        alSourcePlay(sourceObj_->Get());
    }
}

void ALSound::Pause()
{
    if (sourceObj_)
        alSourcePause(sourceObj_->Get());
}

void ALSound::Stop()
{
    if (sourceObj_)
        alSourceStop(sourceObj_->Get());
}

void ALSound::SetLooping(bool enable)
{
    if (!GetStreamSource())
    {
        auto looping = ALBool(enable);
        if (sourceLooping_ != looping)
        {
            sourceLooping_ = looping;
            if (sourceObj_)
                sourceObj_->SetInt(AL_LOOPING, sourceLooping_);
        }
    }
}

bool ALSound::GetLooping() const
{
    return (sourceLooping_ != AL_FALSE);
}

void ALSound::SetVolume(float volume)
{
    sourceGain_ = volume;
    if (sourceObj_)
        sourceObj_->SetFloat(AL_GAIN, sourceGain_);
}

float ALSound::GetVolume() const
{
    return sourceGain_;
}

void ALSound::SetPitch(float pitch)
{
    sourcePitch_ = pitch;
    if (sourceObj_)
        sourceObj_->SetFloat(AL_PITCH, sourcePitch_);
}

float ALSound::GetPitch() const
{
    return sourcePitch_;
}

bool ALSound::IsPlaying() const
{
    if (sourceObj_)
        return (sourceObj_->GetInt(AL_SOURCE_STATE) == AL_PLAYING);
    else
        return false;
}

bool ALSound::IsPaused() const
{
    if (sourceObj_)
        return (sourceObj_->GetInt(AL_SOURCE_STATE) == AL_PAUSED);
    else
        return false;
}

void ALSound::SetSeek(double position)
{
    if (sourceObj_)
        sourceObj_->SetFloat(AL_SEC_OFFSET, static_cast<float>(position));
}

double ALSound::GetSeek() const
{
    if (sourceObj_)
    {
        auto seek = static_cast<double>(sourceObj_->GetFloat(AL_SEC_OFFSET));
        if (bufferObjQueue_)
            seek += bufferObjQueue_->ProcessedTime();
        return seek;
    }
    return 0.0;
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

    if (sourceObj_)
        sourceObj_->DetachBuffer();

    /* Create new buffer object or fill previous one */
    if (!bufferObj_)
        bufferObj_ = std::make_shared<ALBufferObj>(waveBuffer);
    else
        bufferObj_->BufferData(waveBuffer);

    /* Attach buffer object to source */
    if (sourceObj_)
        sourceObj_->AttachBuffer(*bufferObj_);
}

void ALSound::AttachSharedBuffer(const Sound& sourceBufferSound)
{
    const auto& sourceBufferSoundAL = static_cast<const ALSound&>(sourceBufferSound);

    /* Stop palyback and detach previous buffer object */
    Stop();

    if (sourceObj_)
        sourceObj_->DetachBuffer();

    /* Shared buffer object with the specified source buffer sound */
    bufferObj_ = sourceBufferSoundAL.bufferObj_;

    /* Attach buffer object to source */
    if (sourceObj_ && bufferObj_)
        sourceObj_->AttachBuffer(*bufferObj_);
}

void ALSound::QueueBuffer(const WaveBuffer& waveBuffer)
{
    if (AcquireSourceObj())
    {
        /* Reset possible single buffer object */
        ResetBufferObj();

        /* Initialize buffer object queue (if not done yet) */
        if (!bufferObjQueue_)
            bufferObjQueue_ = std::unique_ptr<ALBufferObjQueue>(new ALBufferObjQueue(sourceObj_->Get()));

        /* Fill buffer data */
        bufferObjQueue_->QueueBufferData(waveBuffer);
    }
}

std::size_t ALSound::GetQueueSize() const
{
    if (sourceObj_)
        return static_cast<std::size_t>(sourceObj_->GetInt(AL_BUFFERS_QUEUED));
    else
        return 0;
}

std::size_t ALSound::GetProcessedQueueSize() const
{
    if (sourceObj_)
        return static_cast<std::size_t>(sourceObj_->GetInt(AL_BUFFERS_PROCESSED));
    else
        return 0;
}

/* ----- 3D sound ----- */

void ALSound::Enable3D(bool enable)
{
    enabled3D_ = enable;
    SetPosition({ 0.0f, 0.0f, 0.0f });
    SetVelocity({ 0.0f, 0.0f, 0.0f });
    SetSpaceRelative(!enable);
}

bool ALSound::Is3DEnabled() const
{
    return enabled3D_;
}

void ALSound::SetPosition(const Gs::Vector3f& position)
{
    if (enabled3D_)
    {
        sourcePosition_ = position;
        if (sourceObj_)
            sourceObj_->SetVector3(AL_POSITION, sourcePosition_);
    }
}

Gs::Vector3f ALSound::GetPosition() const
{
    return sourcePosition_;
}

void ALSound::SetVelocity(const Gs::Vector3f& velocity)
{
    if (enabled3D_)
    {
        sourceVelocity_ = velocity;
        if (sourceObj_)
            sourceObj_->SetVector3(AL_VELOCITY, sourceVelocity_);
    }
}

Gs::Vector3f ALSound::GetVelocity() const
{
    return sourceVelocity_;
}

void ALSound::SetSpaceRelative(bool enable)
{
    if (enabled3D_)
    {
        sourceRealtive_ = ALBool(enable);
        if (sourceObj_)
            sourceObj_->SetInt(AL_SOURCE_RELATIVE, sourceRealtive_);
    }
}

bool ALSound::GetSpaceRelative() const
{
    return (sourceRealtive_ != AL_FALSE);
}

/* ----- Extended functions ----- */

void ALSound::DropSourceObj()
{
    if (sourceObj_)
    {
        alSourceStop(sourceObj_->Get());
        sourceObj_->DetachBuffer();
        sourceObj_ = nullptr;
    }
}


/*
 * ======= Private: =======
 */

void ALSound::ResetBufferObj()
{
    if (bufferObj_)
    {
        /* Detach buffer object from source object */
        if (sourceObj_)
            sourceObj_->DetachBuffer();

        bufferObj_.reset();

        /* Remove OpenAL looping state (only a single buffer can make use of this state) */
        SetLooping(false);
    }
}

void ALSound::ResetBufferObjQueue()
{
    if (bufferObjQueue_)
    {
        /* Reset buffer object queue */
        bufferObjQueue_.reset();

        /* Reset OpenAL looping state (only a single buffer can make use of this state) */
        SetLooping(false);
    }
}

ALSourceObj* ALSound::AcquireSourceObj()
{
    if (!sourceObj_)
    {
        /* Allocate buffer object */
        sourceObj_ = ALSourceObjManager::Instance().AllocSourceObj(*this);

        if (sourceObj_ != nullptr)
        {
            /* Initiaize source object */
            sourceObj_->SetInt(AL_LOOPING, sourceLooping_);
            sourceObj_->SetFloat(AL_GAIN, sourceGain_);
            sourceObj_->SetFloat(AL_PITCH, sourcePitch_);

            if (enabled3D_)
            {
                sourceObj_->SetInt(AL_SOURCE_RELATIVE, sourceRealtive_);
                sourceObj_->SetVector3(AL_POSITION, sourcePosition_);
                sourceObj_->SetVector3(AL_VELOCITY, sourceVelocity_);
            }
            else
            {
                sourceObj_->SetInt(AL_SOURCE_RELATIVE, AL_TRUE);
                sourceObj_->SetVector3(AL_POSITION, { 0.0f, 0.0f, 0.0f });
                sourceObj_->SetVector3(AL_VELOCITY, { 0.0f, 0.0f, 0.0f });
            }

            if (bufferObj_)
                sourceObj_->AttachBuffer(*bufferObj_);
        }
    }
    return sourceObj_;
}


} // /namespace Ac



// ================================================================================
