/*
 * NullSound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NullSound.h"
#include "NullAudioSystem.h"


namespace Ac
{


NullSound::NullSound(NullAudioSystem* audioSystem) :
    audioSystem_ { audioSystem }
{
}

NullSound::~NullSound()
{
    UnregisterInSoundManager();
}

/* ----- Playback ----- */

void NullSound::Play()
{
    if (TotalTime() > 0.0)
    {
        SyncPlay();
        RegisterInSoundManager();
    }
    else
        Stop();
}

void NullSound::Pause()
{
    if (IsPlaying())
    {
        SyncPause();
        UnregisterInSoundManager();
    }
}

void NullSound::Stop()
{
    if (IsPlaying())
    {
        SyncStop();
        UnregisterInSoundManager();
    }
}

void NullSound::SetLooping(bool enable)
{
    std::lock_guard<std::mutex> guard { mutex_ };
    looping_ = enable;
}

bool NullSound::GetLooping() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
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
    std::lock_guard<std::mutex> guard { mutex_ };
    pitch_ = pitch;
}

float NullSound::GetPitch() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
    return pitch_;
}

bool NullSound::IsPlaying() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
    return playing_;
}

bool NullSound::IsPaused() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
    return paused_;
}

void NullSound::SetSeek(double position)
{
    std::lock_guard<std::mutex> guard { mutex_ };
    seek_ = position;
}

double NullSound::GetSeek() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
    return seek_;
}

double NullSound::TotalTime() const
{
    std::lock_guard<std::mutex> guard { mutex_ };
    return UnsynchTotalTime();
}

/* ----- Buffers and streaming ----- */

void NullSound::AttachBuffer(const WaveBuffer& waveBuffer)
{
    std::lock_guard<std::mutex> guard { mutex_ };

    /* Create shared copy of the input buffer and stop current playback */
    waveBuffer_ = std::make_shared<WaveBuffer>(waveBuffer);
    UnsynchStop();
}

void NullSound::AttachSharedBuffer(const Sound& sourceBufferSound)
{
    const auto& sourceBufferSoundNull = static_cast<const NullSound&>(sourceBufferSound);

    std::lock_guard<std::mutex> guard { mutex_ };

    /* Share wave buffer with input sound and stop current playback */
    waveBuffer_ = sourceBufferSoundNull.waveBuffer_;
    UnsynchStop();
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


/*
 * ======= Private: =======
 */

void NullSound::SyncPlay()
{
    std::lock_guard<std::mutex> guard { mutex_ };
    UnsynchPlay();
}

void NullSound::SyncPause()
{
    std::lock_guard<std::mutex> guard { mutex_ };
    UnsynchPause();
}

void NullSound::SyncStop()
{
    std::lock_guard<std::mutex> guard { mutex_ };
    UnsynchStop();
}

void NullSound::UnsynchPlay()
{
    playing_ = true;
    paused_  = false;
}

void NullSound::UnsynchPause()
{
    playing_ = false;
    paused_  = true;
}

void NullSound::UnsynchStop()
{
    playing_    = false;
    paused_     = false;
    seek_       = 0.0;
}

double NullSound::UnsynchTotalTime() const
{
    if (GetStreamSource())
        return GetStreamSource()->TotalTime();
    if (waveBuffer_)
        return waveBuffer_->GetTotalTime();
    return 0.0;
}

void NullSound::RegisterInSoundManager()
{
    audioSystem_->RegisterSound(this);
}

void NullSound::UnregisterInSoundManager()
{
    audioSystem_->UnregisterSound(this);
}


} // /namespace Ac



// ================================================================================
