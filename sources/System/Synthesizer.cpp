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

AC_EXPORT void InitWaveBuffer(WaveBuffer& buffer, double duration, unsigned short channels, unsigned int sampleRate, unsigned short bitsPerSample)
{
    InitWaveBufferFormat(buffer.format, channels, sampleRate, bitsPerSample);
    buffer.buffer.resize(static_cast<std::size_t>(duration * buffer.format.bytesPerSecond));
}
    
template <typename T>
static double DataToSample(T data)
{
    /* Scale sample and clamp into range [min, max] */
    auto sample = static_cast<double>(data);
    
    const auto upperEnd = static_cast<double>(std::numeric_limits<T>::max());
    sample /= upperEnd;
    
    return sample;
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

AC_EXPORT void GenerateWave(
    WaveBuffer& buffer, double phaseBegin, double phaseEnd, const std::function<void(double& sample, double phase)>& waveFunction)
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
    
    union
    {
        void*   raw;
        short*  bits16;
        char*   bits8;
    }
    sampleData;
    
    sampleData.raw = nullptr;
    
    /* Generate since wave for each channel */
    while (blk < blocks && phase <= phaseEnd)
    {
        for (std::size_t chn = 0; chn < channels; ++chn)
        {
            auto sample = 0.0;
            
            /* Read sample from PCM data */
            sampleData.raw = reinterpret_cast<void*>(&pcmData[offset]);
            
            if (bytesPerSample == 2)
                sample = DataToSample(*sampleData.bits16);
            else if (bytesPerSample == 1)
                sample = DataToSample(*sampleData.bits8);
            
            /* Compute current audio sample and clamp to range [-1, 1] */
            waveFunction(sample, phase);
            sample = std::max(-1.0, std::min(sample, 1.0));
            
            /* Write sample to PCM data */
            if (bytesPerSample == 2)
                *sampleData.bits16 = SampleToData<short>(sample);
            else if (bytesPerSample == 1)
                *sampleData.bits8 = SampleToData<char>(sample);
            
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
        [&](double& sample, double phase)
        {
            sample += std::sin((phase + phaseShift)*2.0*M_PI*frequency)*amplitude;
        }
    );
}


} // /namesapce Synthesizer

} // /namespace Ac



// ================================================================================
