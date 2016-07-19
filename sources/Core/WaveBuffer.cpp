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

void WaveBuffer::SetSampleCount(std::size_t sampleCount)
{
    buffer_.resize(sampleCount * format_.BlockAlign());
}

std::size_t WaveBuffer::GetSampleCount() const
{
    auto blockAlign = format_.BlockAlign();
    return (blockAlign > 0 ? BufferSize() / blockAlign : 0);
}

void WaveBuffer::SetTotalTime(double duration)
{
    auto bufferSize = duration * format_.sampleRate;
    SetSampleCount(static_cast<std::size_t>(bufferSize));
}

double WaveBuffer::GetTotalTime() const
{
    return format_.TotalTime(BufferSize());
}

double WaveBuffer::ReadSample(double phase, unsigned short channel) const
{
    double sample = 0.0;

    PCMSampleConst pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(phase, channel);

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

void WaveBuffer::WriteSample(double phase, unsigned short channel, double sample)
{
    PCMSample pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(phase, channel);

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

void WaveBuffer::SetFormat(const WaveBufferFormat& format)
{
    //format_ = format;
    //todo...
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

std::size_t WaveBuffer::GetPCMBufferOffset(double phase, unsigned short channel) const
{
    if (!buffer_.empty() && channel < format_.channels && phase >= 0.0 && phase <= GetTotalTime())
    {
        auto bytesPerSample = (format_.bitsPerSample / 8u);
        auto block          = (static_cast<std::size_t>(phase * static_cast<double>(format_.sampleRate)));
        auto offset         = (block * format_.channels + channel) * bytesPerSample;
        return std::min(offset, BufferSize() - 1u);
    }
    return 0;
}

void* WaveBuffer::GetPCMOffsetPtr(double phase, unsigned short channel)
{
    auto offset = GetPCMBufferOffset(phase, channel);
    return (offset < BufferSize() ? reinterpret_cast<void*>(&buffer_[offset]) : nullptr);
}

const void* WaveBuffer::GetPCMOffsetPtr(double phase, unsigned short channel) const
{
    auto offset = GetPCMBufferOffset(phase, channel);
    return (offset < BufferSize() ? reinterpret_cast<const void*>(&buffer_[offset]) : nullptr);
}


} // /namespace Ac



// ================================================================================
