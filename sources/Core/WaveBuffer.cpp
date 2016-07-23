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

WaveBuffer::WaveBuffer(WaveBuffer&& other) :
    format_( other.format_            ),
    buffer_( std::move(other.buffer_) )
{
}

WaveBuffer& WaveBuffer::operator = (WaveBuffer&& other)
{
    format_ = other.format_;
    buffer_ = std::move(other.buffer_);
    return *this;
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

double WaveBuffer::ReadSample(double timePoint, unsigned short channel) const
{
    return ReadSample(GetIndexFromTimePoint(timePoint), channel);
}

void WaveBuffer::WriteSample(double timePoint, unsigned short channel, double sample)
{
    WriteSample(GetIndexFromTimePoint(timePoint), channel, sample);
}

std::size_t WaveBuffer::GetIndexFromTimePoint(double timePoint) const
{
    /* Clamp time point to range [0, GetTotalTime()) */
    auto sampleCount = GetSampleCount();
    if (sampleCount > 0)
    {
        timePoint   = std::max(0.0, std::min(timePoint, GetTotalTime()));
        auto index  = (static_cast<std::size_t>(timePoint * static_cast<double>(format_.sampleRate)));
        return std::min(index, sampleCount - 1);
    }
    return 0;
}

double WaveBuffer::GetTimePointFromIndex(std::size_t index) const
{
    auto sampleCount = GetSampleCount();
    if (sampleCount > 0)
    {
        index = std::min(index, sampleCount - 1);
        return (static_cast<double>(index) / static_cast<double>(format_.sampleRate));
    }
    return 0.0;
}

void WaveBuffer::SetFormat(const WaveBufferFormat& format)
{
    if (format_ != format)
    {
        if (!buffer_.empty())
        {
            /* Configure temporary buffer with new format */
            WaveBuffer tempBuffer(format);
            if (format_.sampleRate == format.sampleRate)
                tempBuffer.SetSampleCount(GetSampleCount());
            else
                tempBuffer.SetTotalTime(GetTotalTime());

            unsigned short maxChannels = (format_.channels - 1);

            /* Copy samples from current buffer to temporary buffer */
            tempBuffer.ForEachSample(
                [&](double& sample, unsigned short channel, std::size_t index, double timePoint)
                {
                    channel = std::min(channel, maxChannels);
                    if (format_.sampleRate == format.sampleRate)
                        sample = ReadSample(index, channel);
                    else
                        sample = ReadSample(timePoint, channel);
                }
            );

            /* Take temporary buffer as new buffer */
            *this = std::move(tempBuffer);
        }
        else
            format_ = format;
    }
}

void WaveBuffer::SetChannels(unsigned short channels)
{
    auto format = GetFormat();
    format.channels = channels;
    SetFormat(format);
}

/* ----- Sample iteration ----- */

//TODO: abstract this function!!!
void WaveBuffer::ForEachSample(const SampleIterationFunction& iterator, std::size_t indexBegin, std::size_t indexEnd)
{
    /* Validate parameters and clamp range to [0, bufferSize) */
    auto sampleCount = GetSampleCount();
    if (sampleCount == 0 || !iterator)
        return;

    indexBegin  = std::max(std::size_t(0u), std::min(indexBegin, sampleCount - 1u));
    indexEnd    = std::max(indexBegin, std::min(indexEnd, sampleCount - 1u));

    auto channels   = format_.channels;
    auto rate       = format_.sampleRate;

    auto timePoint  = GetTimePointFromIndex(indexBegin);
    auto timeStep   = (1.0 / static_cast<double>(rate));

    for (auto i = indexBegin; i <= indexEnd; ++i)
    {
        for (unsigned short chn = 0; chn < channels; ++chn)
        {
            /* Read sample, modify sample with generator callback, and write sample back to buffer */
            auto sample = ReadSample(i, chn);
            iterator(sample, chn, i, timePoint);
            WriteSample(i, chn, sample);
        }

        /* Increase time point */
        timePoint += timeStep;
    }
}

void WaveBuffer::ForEachSample(const SampleIterationFunction& iterator, double timeBegin, double timeEnd)
{
    ForEachSample(iterator, GetIndexFromTimePoint(timeBegin), GetIndexFromTimePoint(timeEnd));
}

void WaveBuffer::ForEachSample(const SampleIterationFunction& iterator)
{
    auto sampleCount = GetSampleCount();
    if (sampleCount > 0)
        ForEachSample(iterator, 0, sampleCount - 1);
}

//TODO: abstract this function!!!
void WaveBuffer::ForEachSample(const SampleConstIterationFunction& iterator, std::size_t indexBegin, std::size_t indexEnd) const
{
    /* Validate parameters and clamp range to [0, bufferSize) */
    auto sampleCount = GetSampleCount();
    if (sampleCount == 0 || !iterator)
        return;
    
    indexBegin  = std::max(std::size_t(0u), std::min(indexBegin, sampleCount - 1u));
    indexEnd    = std::max(indexBegin, std::min(indexEnd, sampleCount - 1u));
    
    auto channels   = format_.channels;
    auto rate       = format_.sampleRate;
    
    auto timePoint  = GetTimePointFromIndex(indexBegin);
    auto timeStep   = (1.0 / static_cast<double>(rate));
    
    for (auto i = indexBegin; i <= indexEnd; ++i)
    {
        for (unsigned short chn = 0; chn < channels; ++chn)
        {
            /* Read sample and pass to constant iterator */
            iterator(ReadSample(i, chn), chn, i, timePoint);
        }
        
        /* Increase time point */
        timePoint += timeStep;
    }
}

void WaveBuffer::ForEachSample(const SampleConstIterationFunction& iterator, double timeBegin, double timeEnd) const
{
    ForEachSample(iterator, GetIndexFromTimePoint(timeBegin), GetIndexFromTimePoint(timeEnd));
}

void WaveBuffer::ForEachSample(const SampleConstIterationFunction& iterator) const
{
    auto sampleCount = GetSampleCount();
    if (sampleCount > 0)
        ForEachSample(iterator, 0, sampleCount - 1);
}

void WaveBuffer::Append(const WaveBuffer& other)
{
    if (other.GetFormat() != GetFormat())
    {
        /* Adapt format of new buffer (create copy) */
        auto otherCopy = other;
        otherCopy.SetFormat(GetFormat());

        /* Resize this buffer and copy new buffer into this buffer */
        auto prevSize = buffer_.size();
        buffer_.resize(buffer_.size() + otherCopy.buffer_.size());
        std::copy(otherCopy.buffer_.begin(), otherCopy.buffer_.end(), &buffer_[prevSize]);
    }
    else
    {
        /* Resize this buffer and copy new buffer into this buffer */
        auto prevSize = buffer_.size();
        buffer_.resize(buffer_.size() + other.buffer_.size());
        std::copy(other.buffer_.begin(), other.buffer_.end(), &buffer_[prevSize]);
    }
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
