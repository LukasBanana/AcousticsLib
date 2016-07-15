/*
 * WaveBuffer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WaveBuffer.h"

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
    if (format.channels == 2 && buffer.empty())
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
        return numBytes / static_cast<double>(format.bytesPerSecond);
    }
    return 0.0;
}


} // /namespace Ac



// ================================================================================