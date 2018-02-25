/*
 * NullAudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NullAudioSystem.h"
#include "NullSound.h"
#include <algorithm>
#include <chrono>
#include <functional>


namespace Ac
{


NullAudioSystem::NullAudioSystem() :
    sharedLiveFlag_     { std::make_shared<bool>(true)                              },
    soundManagerThread_ { std::bind(&NullAudioSystem::SoundManagerThreadFunc, this) }
{
}

NullAudioSystem::~NullAudioSystem()
{
    ReleaseThread();
    if (soundManagerThread_.joinable())
        soundManagerThread_.join();
}

/* ----- Audio system ----- */

std::string NullAudioSystem::GetVersion() const
{
    return "Null";
}

AudioLimitations NullAudioSystem::GetLimits() const
{
    AudioLimitations limits;
    {
        limits.numSounds = 8096;
    }
    return limits;
}

/* ----- Sounds ----- */

bool NullAudioSystem::CanCreateSound() const
{
    return true;
}

std::unique_ptr<Sound> NullAudioSystem::CreateSound()
{
    return std::unique_ptr<Sound>(new NullSound(this));
}

/* ----- Listener ----- */

void NullAudioSystem::SetListenerPosition(const Gs::Vector3f& position)
{
    listenerPosition_ = position;
}

Gs::Vector3f NullAudioSystem::GetListenerPosition() const
{
    return listenerPosition_;
}

void NullAudioSystem::SetListenerVelocity(const Gs::Vector3f& velocity)
{
    listenerVelocity_ = velocity;
}

Gs::Vector3f NullAudioSystem::GetListenerVelocity() const
{
    return listenerVelocity_;
}

void NullAudioSystem::SetListenerOrientation(const ListenerOrientation& orientation)
{
    listenerOrientation_ = orientation;
}

ListenerOrientation NullAudioSystem::GetListenerOrientation() const
{
    return listenerOrientation_;
}


/*
 * ======= Protected: =======
 */

void NullAudioSystem::RegisterSound(NullSound* sound)
{
    std::lock_guard<std::mutex> guard { soundManagerMutex_ };
    if (std::find(playingSounds_.begin(), playingSounds_.end(), sound) == playingSounds_.end())
        playingSounds_.push_back(sound);
}

void NullAudioSystem::UnregisterSound(NullSound* sound)
{
    std::lock_guard<std::mutex> guard { soundManagerMutex_ };
    auto it = std::find(playingSounds_.begin(), playingSounds_.end(), sound);
    if (it != playingSounds_.end())
        playingSounds_.erase(it);
}


/*
 * ======= Private: =======
 */

void NullAudioSystem::SoundManagerThreadFunc()
{
    TimePoint prevTimePoint = std::chrono::system_clock::now();

    while (true)
    {
        /* Determine elapsed time since last check */
        auto currentTimePoint = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedTime = (currentTimePoint - prevTimePoint);
        prevTimePoint = currentTimePoint;

        double timeDelta = elapsedTime.count();

        {
            std::lock_guard<std::mutex> guard { soundManagerMutex_ };

            /* Check if main thread is about to destroy this thread */
            if (release_)
                return;

            /* Update state for each sound */
            playingSounds_.erase(
                std::remove_if(
                    playingSounds_.begin(),
                    playingSounds_.end(),
                    std::bind(&NullAudioSystem::UpdateSoundState, this, std::placeholders::_1, timeDelta)
                ),
                playingSounds_.end()
            );
        }

        /* Yield process by waiting for some time */
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool NullAudioSystem::UpdateSoundState(NullSound* sound, double timeDelta)
{
    bool soundStopped = false;

    /*
    Try to lock mutex, but don't use lock guard, to avoid dead lock between
    thread mutex (NullAudioSystem::soundManagerMutex_) and sound mutex (NullSound::mutex_)
    */
    if (sound->mutex_.try_lock())
    {
        if (!sound->paused_)
        {
            /* Increase playback by time delta and pitch (playback speed factor) */
            sound->seek_ += timeDelta * static_cast<double>(sound->pitch_);

            auto totalTime = sound->UnsynchTotalTime();

            if (totalTime > 0.0)
            {
                if (sound->looping_)
                {
                    /* Wrap seek position inside duration of the track */
                    sound->seek_ = std::fmod(sound->seek_, totalTime);
                }
                else
                {
                    /* Check if sound reached the end of the track */
                    if (sound->seek_ >= totalTime)
                    {
                        sound->UnsynchStop();
                        soundStopped = true;
                    }
                }
            }
        }
        sound->mutex_.unlock();
    }

    return soundStopped;
}

void NullAudioSystem::ReleaseThread()
{
    std::lock_guard<std::mutex> guard { soundManagerMutex_ };
    release_ = true;
}


} // /namespace Ac



// ================================================================================
