/*
 * ALBufferObj.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALBufferObj.h"

#include <Ac/WaveBuffer.h>


namespace Ac
{


static bool ALFormatFromWaveFormat(ALenum& outFormat, const WaveBufferFormat& inFormat)
{
    if (inFormat.bitsPerSample == 16)
    {
        if (inFormat.channels == 2)
            outFormat = AL_FORMAT_STEREO16;
        else if (inFormat.channels == 1)
            outFormat = AL_FORMAT_MONO16;
        else
            return false;
    }
    else if (inFormat.bitsPerSample == 8)
    {
        if (inFormat.channels == 2)
            outFormat = AL_FORMAT_STEREO8;
        else if (inFormat.channels == 1)
            outFormat = AL_FORMAT_MONO8;
        else
            return false;
    }
    else
        return false;
    return true;
}

ALBufferObj::ALBufferObj()
{
    alGenBuffers(1, &handle_);
    if (alGetError() != AL_NO_ERROR)
        throw std::runtime_error("failed to generate OpenAL buffer object");
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
    unsigned short channels = 0, bitsPerSample = 0;
    
    switch (format)
    {
        case AL_FORMAT_MONO8:
            channels        = 1;
            bitsPerSample   = 8;
            break;
        case AL_FORMAT_MONO16:
            channels        = 1;
            bitsPerSample   = 16;
            break;
        case AL_FORMAT_STEREO8:
            channels        = 2;
            bitsPerSample   = 8;
            break;
        case AL_FORMAT_STEREO16:
            channels        = 2;
            bitsPerSample   = 16;
            break;
    }
    
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
