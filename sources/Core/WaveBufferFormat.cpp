/*
 * WaveBufferFormat.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/WaveBufferFormat.h>


namespace Ac
{


WaveBufferFormat::WaveBufferFormat(std::uint32_t sampleRate, std::uint16_t bitsPerSample, std::uint16_t channels) :
    sampleRate    { sampleRate    },
    bitsPerSample { bitsPerSample },
    channels      { channels      }
{
}

std::size_t WaveBufferFormat::BytesPerFrame() const
{
    return (channels * bitsPerSample) / 8;
}

std::size_t WaveBufferFormat::BytesPerSecond() const
{
    return (sampleRate * BytesPerFrame());
}

double WaveBufferFormat::TotalTime(std::size_t bufferSize) const
{
    auto bytesPerSecond = BytesPerSecond();
    return (bytesPerSecond > 0 ? static_cast<double>(bufferSize) / bytesPerSecond : 0.0);
}

bool WaveBufferFormat::IsSigned() const
{
    return (bitsPerSample > 8);
}


AC_EXPORT bool operator == (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs)
{
    return
        ( lhs.sampleRate    == rhs.sampleRate    ) &&
        ( lhs.bitsPerSample == rhs.bitsPerSample ) &&
        ( lhs.channels      == rhs.channels      );
}

AC_EXPORT bool operator != (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs)
{
    return !(lhs == rhs);
}


} // /namespace Ac



// ================================================================================
