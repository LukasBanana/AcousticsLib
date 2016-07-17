/*
 * ALBufferObj.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALBufferObj.h"
#include "../../Core/WaveBuffer.h"


namespace Ac
{


static bool ALFormatFromWaveFormat(const WaveFormat& waveFormat, ALenum& outputFormat)
{
    if (waveFormat.bitsPerSample == 16)
    {
        if (waveFormat.channels == 2)
            outputFormat = AL_FORMAT_STEREO16;
        else if (waveFormat.channels == 1)
            outputFormat = AL_FORMAT_MONO16;
        else
            return false;
    }
    else if (waveFormat.bitsPerSample == 8)
    {
        if (waveFormat.channels == 2)
            outputFormat = AL_FORMAT_STEREO8;
        else if (waveFormat.channels == 1)
            outputFormat = AL_FORMAT_MONO8;
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
    std::size_t channels = 0, bitsPerSample = 0;
    
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
    
    totalTime_ = WaveBuffer::TotalTime(size, sampleRate, channels, bitsPerSample);
}

void ALBufferObj::BufferData(const WaveBuffer& waveBuffer)
{
    ALenum outputFormat = 0;
    if (ALFormatFromWaveFormat(waveBuffer.format, outputFormat))
    {
        BufferData(
            outputFormat,
            waveBuffer.buffer.data(),
            waveBuffer.buffer.size(),
            waveBuffer.format.sampleRate
        );
    }
}


} // /namespace Ac



// ================================================================================
