/*
 * ALBufferObj.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALBufferObj.h"
#include "ALFormat.h"


namespace Ac
{


ALBufferObj::ALBufferObj()
{
    alGenBuffers(1, &handle_);

    auto err = alGetError();
    if (err != AL_NO_ERROR)
        throw std::runtime_error("failed to generate OpenAL buffer object (" + ALErrorToString(err) + ")");
}

ALBufferObj::ALBufferObj(const WaveBuffer& waveBuffer) :
    ALBufferObj {}
{
    BufferData(waveBuffer);
}

ALBufferObj::~ALBufferObj()
{
    if (handle_ != 0 && alIsBuffer(handle_))
        alDeleteBuffers(1, &handle_);
}

void ALBufferObj::BufferData(ALenum format, const ALvoid* buffer, ALsizei size, ALsizei sampleRate)
{
    /* Fill buffer data */
    alBufferData(handle_, format, buffer, size, sampleRate);

    /* Compute total buffer time in seconds */
    std::uint16_t channels = 0, bitsPerSample = 0;
    WaveFormatFromALFormat(format, channels, bitsPerSample);

    totalTime_ = WaveBufferFormat(sampleRate, channels, bitsPerSample).TotalTime(size);
}

void ALBufferObj::BufferData(const WaveBuffer& waveBuffer)
{
    ALenum fmt = 0;
    if (ALFormatFromWaveFormat(fmt, waveBuffer.GetFormat()))
    {
        BufferData(
            fmt,
            waveBuffer.Data(),
            waveBuffer.BufferSize(),
            waveBuffer.GetFormat().sampleRate
        );
    }
}


} // /namespace Ac



// ================================================================================
