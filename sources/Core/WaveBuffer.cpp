/*
 * WaveBuffer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PCMData.h"

#include <Ac/WaveBuffer.h>
#include <algorithm>


namespace Ac
{


WaveBuffer::WaveBuffer(const WaveBufferFormat& format) :
    format_( format )
{
}

std::size_t WaveBuffer::GetSampleCount() const
{
    auto blockAlign = format_.BlockAlign();
    return (blockAlign > 0 ? BufferSize() / blockAlign : 0);
}

void WaveBuffer::SetSampleCount(std::size_t sampleCount)
{
    /* Resize buffer and initialize with 0 for signed formats and with 127 for 8-bit unsigned format */
    auto bufferSize = sampleCount * format_.BlockAlign();
    if (format_.IsSigned())
        buffer_.resize(bufferSize, 0);
    else
        buffer_.resize(bufferSize, 127);
}

double WaveBuffer::GetTotalTime() const
{
    return format_.TotalTime(BufferSize());
}

void WaveBuffer::SetTotalTime(double duration)
{
    auto bufferSize = duration * format_.sampleRate;
    SetSampleCount(static_cast<std::size_t>(bufferSize));
}

double WaveBuffer::ReadSample(std::size_t index, unsigned short channel) const
{
    double sample = 0.0;

    PCMSampleConst pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(GetPCMBufferOffset(index, channel));

    if (pcmSample.raw)
    {
        switch (format_.bitsPerSample)
        {
            case 16:
                PCMDataToSample(sample, *pcmSample.bits16);
                break;
            case 8:
                PCMDataToSample(sample, *pcmSample.bits8);
                break;
        }
    }

    return sample;
}

void WaveBuffer::WriteSample(std::size_t index, unsigned short channel, double sample)
{
    PCMSample pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(GetPCMBufferOffset(index, channel));

    if (pcmSample.raw)
    {
        switch (format_.bitsPerSample)
        {
            case 16:
                SampleToPCMData(*pcmSample.bits16, sample);
                break;
            case 8:
                SampleToPCMData(*pcmSample.bits8, sample);
                break;
        }
    }
}

double WaveBuffer::ReadSample(double phase, unsigned short channel) const
{
    return ReadSample(GetIndexFromPhase(phase), channel);
}

void WaveBuffer::WriteSample(double phase, unsigned short channel, double sample)
{
    WriteSample(GetIndexFromPhase(phase), channel, sample);
}

std::size_t WaveBuffer::GetIndexFromPhase(double phase) const
{
    /* Clamp phase to range [0, GetTotalTime()) */
    if (!buffer_.empty())
    {
        phase       = std::max(0.0, std::min(phase, GetTotalTime()));
        auto index  = (static_cast<std::size_t>(phase * static_cast<double>(format_.sampleRate)));
        return std::min(index, GetSampleCount() - 1);
    }
    return 0;
}

void WaveBuffer::SetFormat(const WaveBufferFormat& format)
{
    if (format_ != format)
    {
        format_ = format;
        //todo...
    }
}

void WaveBuffer::SetChannels(unsigned short channels)
{
    auto format = GetFormat();
    format.channels = channels;
    SetFormat(format);
}


/*
 * ======= Private: =======
 */

std::size_t WaveBuffer::GetPCMBufferOffset(std::size_t index, unsigned short channel) const
{
    /* Scale index by sample block alignment and append channel offset */
    auto channelOffset = (channel < format_.channels ? channel * format_.bitsPerSample / 8 : 0);
    return (index * format_.BlockAlign() + channelOffset);
}

void* WaveBuffer::GetPCMOffsetPtr(std::size_t offset)
{
    return (offset < BufferSize() ? reinterpret_cast<void*>(&buffer_[offset]) : nullptr);
}

const void* WaveBuffer::GetPCMOffsetPtr(std::size_t offset) const
{
    return (offset < BufferSize() ? reinterpret_cast<const void*>(&buffer_[offset]) : nullptr);
}


} // /namespace Ac



// ================================================================================
