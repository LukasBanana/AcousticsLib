/*
 * NullSound.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_NULL_SOUND_H
#define AC_NULL_SOUND_H


#include <Ac/Sound.h>
#include <mutex>


namespace Ac
{


class NullAudioSystem;

class NullSound : public Sound
{

    public:

        NullSound(NullAudioSystem* audioSystem);
        ~NullSound();

        /* ----- Playback ----- */

        void Play() override;
        void Pause() override;
        void Stop() override;

        void SetLooping(bool enable) override;
        bool GetLooping() const override;

        void SetVolume(float volume) override;
        float GetVolume() const override;

        void SetPitch(float pitch) override;
        float GetPitch() const override;

        bool IsPlaying() const override;
        bool IsPaused() const override;

        void SetSeek(double position) override;
        double GetSeek() const override;

        double TotalTime() const override;

        /* ----- Buffers and streaming ----- */

        void AttachBuffer(const WaveBuffer& waveBuffer) override;
        void AttachSharedBuffer(const Sound& sourceBufferSound) override;
        void QueueBuffer(const WaveBuffer& waveBuffer) override;

        std::size_t GetQueueSize() const override;
        std::size_t GetProcessedQueueSize() const override;

        /* ----- 3D sound ----- */

        void Enable3D(bool enable = true) override;
        bool Is3DEnabled() const override;

        void SetPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetPosition() const override;

        void SetVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetVelocity() const override;

        void SetSpaceRelative(bool enable) override;
        bool GetSpaceRelative() const override;

    private:

        friend class NullAudioSystem;

        void SyncPlay();
        void SyncPause();
        void SyncStop();

        void UnsynchPlay();
        void UnsynchPause();
        void UnsynchStop();

        double UnsynchTotalTime() const;

        void RegisterInSoundManager();
        void UnregisterInSoundManager();

        NullAudioSystem*            audioSystem_    = nullptr;

        mutable std::mutex          mutex_;

        // synchronized {
        std::shared_ptr<WaveBuffer> waveBuffer_;
        double                      seek_           = 0.0f;
        bool                        playing_        = false;
        bool                        paused_         = false;
        bool                        looping_        = false;
        float                       pitch_          = 1.0f;
        // }

        float                       volume_         = 1.0f;
        bool                        enabled3D_      = false;
        bool                        spaceRelative_  = false;
        Gs::Vector3f                position_;
        Gs::Vector3f                velocity_;

};


} // /namespace Ac


#endif



// ================================================================================
