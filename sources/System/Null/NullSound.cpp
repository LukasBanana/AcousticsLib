/*
 * NullSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NullSound.h"


namespace Ac
{


/* ----- Playback ----- */

void NullSound::Play()
{
    playing_    = true;
    paused_     = false;
}

void NullSound::Pause()
{
    paused_     = true;
}

void NullSound::Stop()
{
    playing_    = false;
    paused_     = false;
    seek_       = 0.0;
}

void NullSound::SetLooping(bool enable)
{
    looping_ = enable;
}

bool NullSound::GetLooping() const
{
    return looping_;
}

void NullSound::SetVolume(float volume)
{
    volume_ = volume;
}

float NullSound::GetVolume() const
{
    return volume_;
}

void NullSound::SetPitch(float pitch)
{
    pitch_ = pitch;
}

float NullSound::GetPitch() const
{
    return pitch_;
}

bool NullSound::IsPlaying() const
{
    return playing_;
}

bool NullSound::IsPaused() const
{
    return paused_;
}

void NullSound::SetSeek(double position)
{
    seek_ = position;
}

double NullSound::GetSeek() const
{
    return seek_;
}

double NullSound::TotalTime() const
{
    if (GetStreamSource())
        return GetStreamSource()->TotalTime();
    if (waveBuffer_)
        return waveBuffer_->GetTotalTime();
    return 0.0;
}

/* ----- Buffers and streaming ----- */

void NullSound::AttachBuffer(const WaveBuffer& waveBuffer)
{
    /* Create shared copy of the input buffer */
    waveBuffer_ = std::make_shared<WaveBuffer>(waveBuffer);

    /* Stop palyback */
    Stop();
}

void NullSound::AttachSharedBuffer(const Sound& sourceBufferSound)
{
    const auto& sourceBufferSoundNull = static_cast<const NullSound&>(sourceBufferSound);

    /* Share wave buffer with input sound */
    waveBuffer_ = sourceBufferSoundNull.waveBuffer_;

    /* Stop palyback and detach previous buffer object */
    Stop();
}

void NullSound::QueueBuffer(const WaveBuffer& waveBuffer)
{
    // dummy
}

std::size_t NullSound::GetQueueSize() const
{
    return 0u; // dummy
}

std::size_t NullSound::GetProcessedQueueSize() const
{
    return 0u; // dummy
}

/* ----- 3D sound ----- */

void NullSound::Enable3D(bool enable)
{
    enabled3D_ = enable;
}

bool NullSound::Is3DEnabled() const
{
    return enabled3D_;
}

void NullSound::SetPosition(const Gs::Vector3f& position)
{
    if (enabled3D_)
        position_ = position;
}

Gs::Vector3f NullSound::GetPosition() const
{
    return position_;
}

void NullSound::SetVelocity(const Gs::Vector3f& velocity)
{
    if (enabled3D_)
        velocity_ = velocity;
}

Gs::Vector3f NullSound::GetVelocity() const
{
    return velocity_;
}

void NullSound::SetSpaceRelative(bool enable)
{
    if (enabled3D_)
        spaceRelative_ = enable;
}

bool NullSound::GetSpaceRelative() const
{
    return spaceRelative_;
}


} // /namespace Ac



// ================================================================================
