/*
 * XA2Sound.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_XA2_SOUND_H
#define AC_XA2_SOUND_H


#include <Ac/Sound.h>
#include <xaudio2.h>
#include <memory>
#include "ComPtr.h"


namespace Ac
{


class XA2Sound : public Sound
{

    public:

        XA2Sound(IXAudio2* device);
        ~XA2Sound();

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

        void CreateSourceVoice(const WaveBufferFormat& format, UINT32 flags = 0);
        void CreateSourceVoiceForBuffer(const std::shared_ptr<WaveBuffer>& waveBuffer);
        void SubmitBuffer();

        UINT64 GetSamplesPlayer() const;

        IXAudio2*                   device_         = nullptr;
        IXAudio2SourceVoice*        sourceVoice_    = nullptr;

        UINT64                      sampleRate_     = 0u;
        bool                        paused_         = false;

        std::shared_ptr<WaveBuffer> waveBuffer_;

};


} // /namespace Ac


#endif



// ================================================================================
