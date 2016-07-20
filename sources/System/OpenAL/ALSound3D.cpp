/*
 * ALSound3D.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSound3D.h"

#include <cmath>
#include <algorithm>


namespace Ac
{


ALSound3D::ALSound3D()
{
}

ALSound3D::~ALSound3D()
{
}

void ALSound3D::Play()
{
    alSourcePlay(sourceObj_.Get());
}

void ALSound3D::Pause()
{
    alSourcePause(sourceObj_.Get());
}

void ALSound3D::Stop()
{
    alSourceStop(sourceObj_.Get());
}

void ALSound3D::SetVolume(float volume)
{
    sourceObj_.SetFloat(AL_GAIN, volume);
}

float ALSound3D::GetVolume() const
{
    return sourceObj_.GetFloat(AL_GAIN);
}

void ALSound3D::SetPitch(float pitch)
{
    sourceObj_.SetFloat(AL_PITCH, pitch);
}

float ALSound3D::GetPitch() const
{
    return sourceObj_.GetFloat(AL_PITCH);
}

bool ALSound3D::IsPlaying() const
{
    return (sourceObj_.GetInt(AL_SOURCE_STATE) == AL_PLAYING);
}

bool ALSound3D::IsPaused() const
{
    return (sourceObj_.GetInt(AL_SOURCE_STATE) == AL_PAUSED);
}

void ALSound3D::SetSeek(double position)
{
    sourceObj_.SetFloat(AL_SEC_OFFSET, static_cast<float>(position));
}

double ALSound3D::GetSeek() const
{
    return static_cast<double>(sourceObj_.GetFloat(AL_SEC_OFFSET));
}

double ALSound3D::TotalTime() const
{
    return (bufferObj_ ? bufferObj_->GetTotalTime() : 0.0);
}

void ALSound3D::SetPosition(const Gs::Vector3f& position)
{
    sourceObj_.SetVector3(AL_POSITION, position);
}

Gs::Vector3f ALSound3D::GetPosition() const
{
    return sourceObj_.GetVector3(AL_POSITION);
}

void ALSound3D::SetVelocity(const Gs::Vector3f& velocity)
{
    sourceObj_.SetVector3(AL_VELOCITY, velocity);
}

Gs::Vector3f ALSound3D::GetVelocity() const
{
    return sourceObj_.GetVector3(AL_VELOCITY);
}

void ALSound3D::SetSpaceRelative(bool enable)
{
    sourceObj_.SetInt(AL_SOURCE_RELATIVE, (enable ? AL_TRUE : AL_FALSE));
}

bool ALSound3D::GetSpaceRelative() const
{
    return (sourceObj_.GetInt(AL_SOURCE_RELATIVE) != AL_FALSE);
}

void ALSound3D::AttachBuffer(const WaveBuffer& waveBuffer)
{
    bufferObj_ = std::make_shared<ALBufferObj>();

    if (waveBuffer.GetFormat().channels != 1)
    {
        /* OpenAL only allows one channel for 3D sounds, so convert it */
        auto waveBufferCopy = waveBuffer;
        waveBufferCopy.SetChannels(1);
        bufferObj_->BufferData(waveBufferCopy);
    }
    else
        bufferObj_->BufferData(waveBuffer);

    sourceObj_.AttachBuffer(*bufferObj_);
}

void ALSound3D::QueueBuffer(const WaveBuffer& waveBuffer)
{
    //todo...
}

std::size_t ALSound3D::GetQueueSize() const
{
    return 0;//static_cast<std::size_t>(sourceObj_.GetInt(AL_BUFFERS_QUEUED));
}

std::size_t ALSound3D::GetProcessedQueueSize() const
{
    return 0;//static_cast<std::size_t>(sourceObj_.GetInt(AL_BUFFERS_PROCESSED));
}


} // /namespace Ac



// ================================================================================
