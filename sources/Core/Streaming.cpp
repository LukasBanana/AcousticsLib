/*
 * Streaming.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Streaming.h"


namespace Ac
{


static WaveBuffer g_commonStreamingBuffer;

static void EnsureNonEmptyBuffer(WaveBuffer& waveBuffer, const WaveBufferFormat& format)
{
    /* Allocate a default wave buffer if the input buffer is empty */
    if (waveBuffer.GetSampleCount() == 0)
    {
        waveBuffer.SetFormat(format);
        waveBuffer.SetTotalTime(0.25);
    }
}

AC_EXPORT void InitStreaming(Sound& sound, double startTime, std::size_t queueAdvanceSize)
{
    const auto& stream = sound.GetStreamSource();
    if (stream)
    {
        EnsureNonEmptyBuffer(g_commonStreamingBuffer, stream->GetFormat());

        stream->Seek(startTime);

        /* Load 'queueAdvanceSize' buffers in advance */
        while (queueAdvanceSize-- > 0)
        {
            auto bytes = stream->StreamWaveBuffer(g_commonStreamingBuffer);
            if (bytes > 0)
                sound.QueueBuffer(g_commonStreamingBuffer);
            else
                break;
        }
    }
}

AC_EXPORT void Streaming(Sound& sound, WaveBuffer& waveBuffer)
{
    const auto& stream = sound.GetStreamSource();
    if (stream)
    {
        EnsureNonEmptyBuffer(waveBuffer, stream->GetFormat());

        /* Process audio streaming */
        while (sound.GetProcessedQueueSize() > 0)
        {
            auto bytes = stream->StreamWaveBuffer(waveBuffer);
            if (bytes > 0)
                sound.QueueBuffer(waveBuffer);
            else
                break;
        }
    }
}

AC_EXPORT void Streaming(Sound& sound)
{
    Streaming(sound, g_commonStreamingBuffer);
}


} // /namespace Ac



// ================================================================================
