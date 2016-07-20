/*
 * ALSound3D.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_SOUND3D_H__
#define __AC_AL_SOUND3D_H__


#include "ALSourceObj.h"
#include "ALBufferObj.h"

#include <Ac/Sound3D.h>
#include <vector>


namespace Ac
{


class ALSound3D : public Sound3D
{

    public:

        ALSound3D();
        ~ALSound3D();

        void Play() override;
        void Pause() override;
        void Stop() override;

        void SetVolume(float volume) override;
        float GetVolume() const override;

        void SetPitch(float pitch) override;
        float GetPitch() const override;

        bool IsPlaying() const override;
        bool IsPaused() const override;

        void SetSeek(double position) override;
        double GetSeek() const override;

        double TotalTime() const override;

        void AttachBuffer(const WaveBuffer& waveBuffer) override;
        void QueueBuffer(const WaveBuffer& waveBuffer) override;

        std::size_t GetQueueSize() const override;
        std::size_t GetProcessedQueueSize() const override;

        void SetPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetPosition() const override;

        void SetVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetVelocity() const override;

        void SetSpaceRelative(bool enable) override;
        bool GetSpaceRelative() const override;

    private:

        ALSourceObj                     sourceObj_;
        std::shared_ptr<ALBufferObj>    bufferObj_;

};


} // /namespace Ac


#endif



// ================================================================================
