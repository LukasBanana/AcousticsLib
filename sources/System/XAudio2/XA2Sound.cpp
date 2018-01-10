/*
 * XA2Sound.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2Sound.h"


namespace Ac
{


XA2Sound::XA2Sound(IXAudio2* device) :
    device_ { device }
{
}

XA2Sound::~XA2Sound()
{
    if (sourceVoice_)
        sourceVoice_->DestroyVoice();
}

/* ----- Playback ----- */

void XA2Sound::Play()
{
    if (sourceVoice_)
        sourceVoice_->Start();
}

void XA2Sound::Pause()
{
}

void XA2Sound::Stop()
{
    if (sourceVoice_)
        sourceVoice_->Stop();
}

void XA2Sound::SetLooping(bool enable)
{
    if (sourceVoice_)
    {
        if (!enable)
            sourceVoice_->ExitLoop();
    }
}

bool XA2Sound::GetLooping() const
{
    return false;
}

void XA2Sound::SetVolume(float volume)
{
    if (sourceVoice_)
        sourceVoice_->SetVolume(std::max(0.0f, volume));
}

float XA2Sound::GetVolume() const
{
    float volume = 0.0f;
    if (sourceVoice_)
        sourceVoice_->GetVolume(&volume);
    return volume;
}

void XA2Sound::SetPitch(float pitch)
{
    if (sourceVoice_)
        sourceVoice_->SetFrequencyRatio(Gs::Clamp(pitch, XAUDIO2_MIN_FREQ_RATIO, XAUDIO2_MAX_FREQ_RATIO));
}

float XA2Sound::GetPitch() const
{
    float pitch = 1.0f;
    if (sourceVoice_)
        sourceVoice_->GetFrequencyRatio(&pitch);
    return pitch;
}

bool XA2Sound::IsPlaying() const
{
    return false;
}

bool XA2Sound::IsPaused() const
{
    return false;
}

void XA2Sound::SetSeek(double position)
{
}

double XA2Sound::GetSeek() const
{
    if (sourceVoice_ && sampleRate_ > 0)
    {
        XAUDIO2_VOICE_STATE state;
        sourceVoice_->GetState(&state, 0);
        return (static_cast<double>(state.SamplesPlayed) / static_cast<double>(sampleRate_));
    }
    return 0.0;
}

double XA2Sound::TotalTime() const
{
    if (GetStreamSource())
        return GetStreamSource()->TotalTime();
    /*if (bufferObj_)
        return bufferObj_->GetTotalTime();*/
    return 0.0;
}

/* ----- Buffers and streaming ----- */

void XA2Sound::AttachBuffer(const WaveBuffer& waveBuffer)
{
    CreateSourceVoice(waveBuffer.GetFormat());

    /* Build XAudio2 buffer descriptor */
    XAUDIO2_BUFFER bufferDesc;
    {
        bufferDesc.Flags        = XAUDIO2_END_OF_STREAM;
        bufferDesc.AudioBytes   = static_cast<UINT32>(waveBuffer.BufferSize());
        bufferDesc.pAudioData   = reinterpret_cast<const BYTE*>(waveBuffer.Data());
        bufferDesc.PlayBegin    = 0;
        bufferDesc.PlayLength   = 0; // play entire buffer
        bufferDesc.LoopBegin    = 0;
        bufferDesc.LoopLength   = 0;
        bufferDesc.LoopCount    = 0;
        bufferDesc.pContext     = this;
    }

    /* Submit XAudio2 source buffer */
    if (sourceVoice_->SubmitSourceBuffer(&bufferDesc) != S_OK)
        throw std::runtime_error("failed to submit XAudio2 source buffer to source voice");
}

void XA2Sound::AttachSharedBuffer(const Sound& sourceBufferSound)
{
}

void XA2Sound::QueueBuffer(const WaveBuffer& waveBuffer)
{
}

std::size_t XA2Sound::GetQueueSize() const
{
    return 0u;
}

std::size_t XA2Sound::GetProcessedQueueSize() const
{
    return 0u;
}

/* ----- 3D sound ----- */

void XA2Sound::Enable3D(bool enable)
{
}

bool XA2Sound::Is3DEnabled() const
{
    return false;
}

void XA2Sound::SetPosition(const Gs::Vector3f& position)
{
}

Gs::Vector3f XA2Sound::GetPosition() const
{
    return {};
}

void XA2Sound::SetVelocity(const Gs::Vector3f& velocity)
{
}

Gs::Vector3f XA2Sound::GetVelocity() const
{
    return {};
}

void XA2Sound::SetSpaceRelative(bool enable)
{
}

bool XA2Sound::GetSpaceRelative() const
{
    return false;
}


/*
 * ======= Private: =======
 */

void XA2Sound::CreateSourceVoice(const WaveBufferFormat& format, UINT32 flags)
{
    /* Convert wave buffer format into WAVEFORMATEX structure */
    WAVEFORMATEX sourceFormat;
    {
        sourceFormat.wFormatTag         = WAVE_FORMAT_PCM;
        sourceFormat.nChannels          = static_cast<WORD>(format.channels);
        sourceFormat.nSamplesPerSec     = static_cast<DWORD>(format.sampleRate);
        sourceFormat.nAvgBytesPerSec    = static_cast<DWORD>(format.BytesPerSecond());
        sourceFormat.nBlockAlign        = static_cast<WORD>(format.BytesPerFrame());
        sourceFormat.wBitsPerSample     = static_cast<WORD>(format.bitsPerSample);
        sourceFormat.cbSize             = 0; // no extra bytes
    }

    /* Store sample rate to transform seek position from samples into seconds */
    sampleRate_ = static_cast<UINT64>(format.sampleRate);

    /* Release previous voice */
    if (sourceVoice_)
        sourceVoice_->DestroyVoice();

    /* Create XAudio2 source voice */
    if (device_->CreateSourceVoice(&sourceVoice_, &sourceFormat, flags) != S_OK)
        throw std::runtime_error("failed to create XAudio2 source voice");
}


} // /namespace Ac



// ================================================================================
