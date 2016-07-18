/*
 * WaveBuffer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PCMData.h"

#include <Ac/WaveBuffer.h>
#include <Ac/WaveFormatTags.h>
#include <algorithm>


namespace Ac
{


static void ChangeFormatChannels(WaveFormat& format, unsigned short channels)
{
    format.channels         = channels;
    format.bytesPerSecond   = format.sampleRate * format.channels * format.bitsPerSample / 8;
    format.blockAlign       = format.channels * format.bitsPerSample / 8;
}

template <typename T>
void ReverseTmpl(const WaveFormat& format, PCMBuffer& buffer)
{
    auto size       = buffer.size();
    auto num        = size / sizeof(T);
    auto numHalf    = num / format.channels / 2;
    auto rawBuffer  = reinterpret_cast<T*>(buffer.data());

    for (std::size_t i = 0, j = 0; i < numHalf; ++i, j += format.channels)
    {
        for (unsigned short chn = 0; chn < format.channels; ++chn)
            std::swap(rawBuffer[chn + j], rawBuffer[num + chn - format.channels - j]);
    }
}

void WaveBuffer::Reverse()
{
    if (!buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                ReverseTmpl<char>(format, buffer);
                break;
            case 16:
                ReverseTmpl<short>(format, buffer);
                break;
        }
    }
}

template <typename T>
void MakeMonoTmpl(WaveFormat& format, PCMBuffer& buffer)
{
    auto size = buffer.size()/2;
    auto num = size / sizeof(T);

    PCMBuffer tempBuffer(size, 0);

    auto rawBufferSrc = reinterpret_cast<const T*>(buffer.data());
    auto rawBufferDst = reinterpret_cast<T*>(tempBuffer.data());

    long long sample;

    for (std::size_t i = 0; i < num; ++i)
    {
        /* Set mono sample to average of the two stereo samples */
        sample = rawBufferSrc[i*2];
        sample += rawBufferSrc[i*2 + 1];
        sample /= 2;

        rawBufferDst[i] = static_cast<T>(sample);
    }
    
    buffer = tempBuffer;
    ChangeFormatChannels(format, 1);
}

void WaveBuffer::MakeMono()
{
    /* Only allow conversion from stereo sound */
    if (format.channels == 2 && !buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                MakeMonoTmpl<char>(format, buffer);
                break;
            case 16:
                MakeMonoTmpl<short>(format, buffer);
                break;
        }
    }
}

template <typename T>
void MakeStereoTmpl(WaveFormat& format, PCMBuffer& buffer)
{
    auto size = buffer.size();
    auto num = size / sizeof(T);

    PCMBuffer tempBuffer(size*2, 0);

    auto rawBufferSrc = reinterpret_cast<const T*>(buffer.data());
    auto rawBufferDst = reinterpret_cast<T*>(tempBuffer.data());

    for (std::size_t i = 0; i < num; ++i)
    {
        /* Copy mono sample to the two stereo samples */
        rawBufferDst[i*2    ] = rawBufferSrc[i];
        rawBufferDst[i*2 + 1] = rawBufferSrc[i];
    }
    
    buffer = tempBuffer;
    ChangeFormatChannels(format, 2);
}

void WaveBuffer::MakeStereo()
{
    /* Only allow conversion from mono sound */
    if (format.channels == 1 && !buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                MakeStereoTmpl<char>(format, buffer);
                break;
            case 16:
                MakeStereoTmpl<short>(format, buffer);
                break;
        }
    }
}

std::size_t WaveBuffer::NumSamples() const
{
    return (format.blockAlign > 0 ? buffer.size() / static_cast<std::size_t>(format.blockAlign) : 0);
}

double WaveBuffer::TotalTime() const
{
    if (format.bytesPerSecond > 0)
    {
        auto numBytes = static_cast<double>(buffer.size());
        return (numBytes / static_cast<double>(format.bytesPerSecond));
    }
    return 0.0;
}

double WaveBuffer::TotalTime(std::size_t bufferSize, std::size_t sampleRate, std::size_t channels, std::size_t bitsPerSample)
{
    auto blockAlign     = (channels * bitsPerSample) / 8;
    auto bytesPerSecond = sampleRate * blockAlign;
    return (bytesPerSecond > 0 ? static_cast<double>(bufferSize) / bytesPerSecond : 0.0);
}

double WaveBuffer::ReadSample(double phase, unsigned short channel) const
{
    double sample = 0.0;

    PCMSampleConst pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(phase, channel);

    if (pcmSample.raw)
    {
        if (format.bitsPerSample == 16)
            PCMDataToSample(sample, *pcmSample.bits16);
        else if (format.bitsPerSample == 8)
            PCMDataToSample(sample, *pcmSample.bits8);
    }

    return sample;
}

void WaveBuffer::WriteSample(double phase, unsigned short channel, double sample)
{
    PCMSample pcmSample;
    pcmSample.raw = GetPCMOffsetPtr(phase, channel);

    if (pcmSample.raw)
    {
        if (format.bitsPerSample == 16)
            SampleToPCMData(*pcmSample.bits16, sample);
        else if (format.bitsPerSample == 8)
            SampleToPCMData(*pcmSample.bits8, sample);
    }
}


/*
 * ======= Private: =======
 */

std::size_t WaveBuffer::GetPCMBufferOffset(double phase, unsigned short channel) const
{
    if (!buffer.empty() && channel < format.channels && phase >= 0.0 && phase <= TotalTime())
    {
        auto bytesPerSample = (format.bitsPerSample / 8);
        auto block          = (static_cast<std::size_t>(phase * static_cast<double>(format.sampleRate)));
        auto offset         = (block * format.channels + channel) * bytesPerSample;
        return std::min(offset, buffer.size() - 1u);
    }
    return 0;
}

void* WaveBuffer::GetPCMOffsetPtr(double phase, unsigned short channel)
{
    auto offset = GetPCMBufferOffset(phase, channel);
    return (offset < buffer.size() ? reinterpret_cast<void*>(&buffer[offset]) : nullptr);
}

const void* WaveBuffer::GetPCMOffsetPtr(double phase, unsigned short channel) const
{
    auto offset = GetPCMBufferOffset(phase, channel);
    return (offset < buffer.size() ? reinterpret_cast<const void*>(&buffer[offset]) : nullptr);
}


} // /namespace Ac



// ================================================================================
