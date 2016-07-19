/*
 * WaveBufferFormat.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/WaveBufferFormat.h>


namespace Ac
{


WaveBufferFormat::WaveBufferFormat(unsigned int sampleRate, unsigned short bitsPerSample, unsigned short channels) :
    sampleRate      ( sampleRate    ),
    bitsPerSample   ( bitsPerSample ),
    channels        ( channels      )
{
}

std::size_t WaveBufferFormat::BlockAlign() const
{
    return (channels * bitsPerSample) / 8;
}

std::size_t WaveBufferFormat::BytesPerSecond() const
{
    return (sampleRate * BlockAlign());
}

double WaveBufferFormat::TotalTime(std::size_t bufferSize) const
{
    auto bytesPerSecond = BytesPerSecond();
    return (bytesPerSecond > 0 ? static_cast<double>(bufferSize) / bytesPerSecond : 0.0);
}


} // /namespace Ac



// ================================================================================
