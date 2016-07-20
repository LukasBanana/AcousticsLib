/*
 * ALSound.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_SOUND_H__
#define __AC_AL_SOUND_H__


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

    private:

        ALSourceObj                         sourceObj_;
        std::shared_ptr<ALBufferObj>        bufferObj_;
        std::unique_ptr<ALBufferObjQueue>   bufferObjQueue_;

        double                              seekOffset_ = 0.0; //!< Offset for the "GetSeek" function, when a buffer queue is used.

};


} // /namespace Ac


#endif



// ================================================================================
