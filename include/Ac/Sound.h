/*
 * Sound.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_SOUND_H__
#define __AC_SOUND_H__


#include "Export.h"
#include "WaveBuffer.h"

#include <string>
#include <memory>


namespace Ac
{


//! Sound source interface.
class AC_EXPORT Sound
{

    public:

        Sound(const Sound&) = delete;
        Sound& operator = (const Sound&) = delete;

        virtual ~Sound()
        {
        }

        //! Starts the sound playback.
        virtual void Play() = 0;
        //! Pauses the sound playback at the current position.
        virtual void Pause() = 0;
        //! Stops the sound playback.
        virtual void Stop() = 0;

        //! Sets the sound volume in the range [0, +inf). By default 1.
        virtual void SetVolume(float volume) = 0;
        //! Returns the sound volume.
        virtual float GetVolume() const = 0;

        //! Sets the sound pitch (or frequency ratio) in the range (0, +inf). By default 1.
        virtual void SetPitch(float pitch) = 0;
        //! Returns the sound pitch.
        virtual float GetPitch() const = 0;

        //! Returns true if the sound is currently being played.
        virtual bool IsPlaying() const = 0;
        //! Returns true if the sound is currently being played but is pause mode.
        virtual bool IsPaused() const = 0;

        //! Seels the current playback position (in seconds) in the range [0, +inf).
        virtual void SetSeek(double position) = 0;
        //! Returns the current playback position (in seconds).
        virtual double GetSeek() const = 0;

        //! Returns the total time (in seconds) this sound takes to be played.
        virtual double TotalTime() const = 0;

        /**
        \brief Attaches the specified wave buffer to this sound.
        \remarks If this function is used, only a single buffer can be added to the sound, and the previous buffer will be removed.
        \see QueueBuffer
        */
        virtual void AttachBuffer(const WaveBuffer& waveBuffer) = 0;

        /**
        \brief Appends the specified buffer at the end of the buffer queue of this sound.
        \remarks If this function is used, the sound will be managed for audio streaming.
        \see AttachBuffer
        */
        virtual void QueueBuffer(const WaveBuffer& waveBuffer) = 0;

    protected:

        Sound() = default;

};


} // /namespace Ac


#endif



// ================================================================================
