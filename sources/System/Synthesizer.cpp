/*
 * Synthesizer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/Synthesizer.h>
#include <Ac/WaveFormatTags.h>
#include <cmath>
#include <algorithm>
#include <limits>


namespace Ac
{

namespace Synthesizer
{


AC_EXPORT void InitWaveBufferFormat(WaveFormat& format, unsigned short channels, unsigned int sampleRate, unsigned short bitsPerSample)
{
    format.formatTag        = WaveFormatTags::PCM;
    format.channels         = channels;
    format.sampleRate       = sampleRate;
    format.bitsPerSample    = bitsPerSample;
    format.blockAlign       = (format.channels * format.bitsPerSample) / 8;
    format.bytesPerSecond   = format.sampleRate * format.blockAlign;
}

AC_EXPORT void InitWaveBuffer(WaveBuffer& buffer, unsigned short channels, unsigned int sampleRate, unsigned short bitsPerSample, double duration)
{
    InitWaveBufferFormat(buffer.format, channels, sampleRate, bitsPerSample);
    buffer.buffer.resize(static_cast<std::size_t>(duration * buffer.format.bytesPerSecond));
}
    
template <typename T>
static T SampleToData(double sample)
{
    /* Scale sample and clamp into range [min, max] */
    const auto lowerEnd = static_cast<double>(std::numeric_limits<T>::lowest());
    const auto upperEnd = static_cast<double>(std::numeric_limits<T>::max());
    
    sample *= upperEnd;
    sample = std::max(lowerEnd, std::min(sample, upperEnd));
    
    return static_cast<T>(sample);
}

AC_EXPORT void GenerateWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, const std::function<double(double phase)>& waveFunction)
{
    if (!waveFunction)
        return;
    
    const auto& format          = buffer.format;
    auto&       pcmData         = buffer.buffer;
    auto        rate            = format.sampleRate;
    
    if (rate == 0 || format.blockAlign == 0)
        return;
    
    auto        bytesPerSample  = format.bitsPerSample/8;
    auto        channels        = format.channels;
    auto        blocks          = pcmData.size() / format.blockAlign;
    
    auto        phase           = phaseBegin;
    auto        phaseStep       = 1.0 / static_cast<double>(rate);
    auto        blk             = static_cast<std::size_t>(phaseBegin*static_cast<double>(rate));
    auto        offset          = blk*channels*bytesPerSample;
    
    /* Generate since wave for each channel */
    while (blk < blocks && phase <= phaseEnd)
    {
        /* Compute current audio sample and clamp to range [-1, 1] */
        auto sample = std::max(-1.0, std::min(waveFunction(phase), 1.0));
        
        for (std::size_t chn = 0; chn < channels; ++chn)
        {
            /* Store sample in PCM data */
            if (bytesPerSample == 2)
            {
                auto data = reinterpret_cast<short*>(&pcmData[offset]);
                *data = SampleToData<short>(sample);
            }
            else if (bytesPerSample == 1)
            {
                auto data = reinterpret_cast<char*>(&pcmData[offset]);
                *data = SampleToData<char>(sample);
            }
            
            offset += bytesPerSample;
        }
        
        /* Increase phase */
        phase += phaseStep;
        ++blk;
    }

}

AC_EXPORT void GenerateSineWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, double amplitude, double phaseShift, double frequency)
{
    GenerateWave(
        buffer, phaseBegin, phaseEnd,
        [&](double phase) -> double
        {
            return std::sin((phase + phaseShift)*2.0*M_PI*frequency)*amplitude;
        }
    );
}


} // /namesapce Synthesizer

} // /namespace Ac



// ================================================================================
