/*
 * ALSound.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_SOUND_H
#define AC_AL_SOUND_H


#include "ALSourceObj.h"
#include "ALBufferObj.h"
#include "ALBufferObjQueue.h"

#include <Ac/Sound.h>
#include <vector>


namespace Ac
{


class ALSound : public Sound
{

    public:

        ALSound();
        ~ALSound();

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

        void ResetBufferObj();
        void ResetBufferObjQueue();

        std::shared_ptr<ALBufferObj>        bufferObj_;
        std::unique_ptr<ALBufferObjQueue>   bufferObjQueue_;

        // Desctructor of source object must be called before destructor of buffer object!
        ALSourceObj                         sourceObj_;

        bool                                enabled3D_  = false;
        bool                                looping_    = false;

};


} // /namespace Ac


#endif



// ================================================================================
