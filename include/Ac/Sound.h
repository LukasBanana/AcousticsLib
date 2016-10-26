/*
 * Sound.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_SOUND_H
#define AC_SOUND_H


#include "Export.h"
#include "WaveBuffer.h"
#include "AudioStream.h"

#include <Gauss/Vector3.h>
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
            // dummy
        }

        /* ----- Playback ----- */

        //! Starts the sound playback.
        virtual void Play() = 0;
        //! Pauses the sound playback at the current position.
        virtual void Pause() = 0;
        //! Stops the sound playback.
        virtual void Stop() = 0;

        /**
        \brief Enables or disables sound looping.
        \param[in] enable If true, the sound will be played from the beginning, after the end was reached. By default false.
        */
        virtual void SetLooping(bool enable) = 0;
        //! Returns true if sound looping is enabled.
        virtual bool GetLooping() const = 0;

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

        /* ----- Buffers and streaming ----- */

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

        /**
        \brief Returns the current size of the buffer queue.
        \see QueueBuffer
        \see GetProcessedQueueSize
        */
        virtual std::size_t GetQueueSize() const = 0;

        /**
        \brief Returns the number of the processed buffer in the queue.
        \remarks This can be used for manual audio streaming if the "AudioSystem::Streaming" function is not used. Example usage:
        \code
        // Initialize buffer queue with 10 buffers
        for (int i = 0; i < 10 && audioStream->StreamWaveBuffer(waveBuffer) > 0; ++i)
            sound->QueueBuffer(waveBuffer);

        // Start continuous streaming
        while (sound->IsPlaying())
        {
            while (sound->GetProcessedQueueSize() > 0)
            {
                if (audioStream->StreamWaveBuffer(waveBuffer) > 0)
                    sound->QueueBuffer(waveBuffer);
                else
                    break;
            }
        }
        \endcode
        \see AudioSystem::Streaming
        */
        virtual std::size_t GetProcessedQueueSize() const = 0;

        /**
        \brief Connects this sound with the specified stream source.
        \remarks This is used for the audio system, to perform continous streaming automatically.
        If you do the streaming manually, you don't necessarily need this function.
        */
        inline void SetStreamSource(const std::shared_ptr<AudioStream>& streamSource)
        {
            streamSource_ = streamSource;
        }

        //! Returns the previously connected stream source.
        inline const std::shared_ptr<AudioStream>& GetStreamSource() const
        {
            return streamSource_;
        }

        /* ----- 3D sound ----- */

        /**
        \brief Enables or disables the 3D sound feature. By default disabled.
        \remarks All 3D sound functions (for position, velocity, and relative space)
        have no effect until this sound was enabled to be a 3D sound.
        Moveover, all 3D attributes (position, velocity, and relative space) are reset whenever this function is called!
        \see SetPosition
        \see SetVelocity
        \see SetSpaceRelative
        */
        virtual void Enable3D(bool enable = true) = 0;
        //! Returns true if 3D sound effect is enbaled.
        virtual bool Is3DEnabled() const = 0;

        //! Sets the world position of this 3D sound. By default (0, 0, 0).
        virtual void SetPosition(const Gs::Vector3f& position) = 0;
        //! Returns the world position of this 3D sound.
        virtual Gs::Vector3f GetPosition() const = 0;

        //! Sets the world velocity of this 3D sound. The velocity is used for the "Doppler"-effect. By default (0, 0, 0).
        virtual void SetVelocity(const Gs::Vector3f& velocity) = 0;
        //! Returns the world velocity of this 3D sound.
        virtual Gs::Vector3f GetVelocity() const = 0;

        //! Specifies whether to make the coordinate space of this sound relative to the listener or not. By default false.
        virtual void SetSpaceRelative(bool enable) = 0;
        //! Returns true if the coordinate space of this sound is relative to the listener or not.
        virtual bool GetSpaceRelative() const = 0;

    protected:

        Sound() = default;

    private:

        std::shared_ptr<AudioStream> streamSource_;

};


} // /namespace Ac


#endif



// ================================================================================
