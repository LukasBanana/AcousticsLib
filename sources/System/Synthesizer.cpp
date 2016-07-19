/*
 * Synthesizer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Core/PCMData.h"

#include <Ac/Synthesizer.h>
//#include <Ac/WaveFormatTags.h>
#include <cmath>


using namespace std::placeholders;

namespace Ac
{

namespace Synthesizer
{


// see https://en.wikipedia.org/wiki/Piano_key_frequencies
AC_EXPORT double GetNoteFrequency(const MusicalNotes note, int interval)
{
    /* Determine key number */
    int noteValue = static_cast<int>(note);

    int keyNo = (interval * 12 + noteValue - 8);
    keyNo = std::max(1, std::min(keyNo, 88));

    /* Compute frequency by key number */
    double baseFreq = 440.0;
    double root_12_of_2 = std::pow(2.0, 1.0/12.0);
    double freq = std::pow(root_12_of_2, keyNo - 49) * baseFreq;

    return freq;
}

#if 0

AC_EXPORT void GenerateWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, const WaveGeneratorFunction& waveGenerator)
{
    if (!waveGenerator)
        return;
    
    phaseBegin      = std::max(0.0, phaseBegin);
    phaseEnd        = std::max(phaseBegin, phaseEnd);

    auto channels   = buffer.GetFormat().channels;
    auto rate       = buffer.GetFormat().sampleRate;
    auto idxBegin   = buffer.GetIndexFromPhase(phaseBegin);
    auto idxEnd     = buffer.GetIndexFromPhase(phaseEnd);

    auto phase      = phaseBegin;
    auto phaseStep  = (1.0 / static_cast<double>(rate));

    for (std::size_t i = idxBegin; i < idxEnd; ++i)
    {
        for (unsigned short chn = 0; chn < channels; ++chn)
        {
            /* Read sample, modify sample with generator callback, and write sample back to buffer */
            auto sample = buffer.ReadSample(i, chn);
            waveGenerator(sample, chn, phase);
            buffer.WriteSample(i, chn, sample);
        }

        /* Increase phase */
        phase += phaseStep;
    }
}

AC_EXPORT void GenerateWave(WaveBuffer& buffer, const WaveGeneratorFunction& waveGenerator)
{
    GenerateWave(buffer, 0.0, buffer.GetTotalTime(), waveGenerator);
}

#endif

static void SineWaveGeneratorCallback(
    double& sample, unsigned short channel, std::size_t index, double phase,
    double amplitude, double phaseShift, double frequency)
{
    sample += std::sin((phase + phaseShift)*2.0*M_PI*frequency)*amplitude;
}

AC_EXPORT SampleIterationFunction SineWaveGenerator(double amplitude, double phaseShift, double frequency)
{
    return std::bind(SineWaveGeneratorCallback, _1, _2, _3, _4, amplitude, phaseShift, frequency);
}

static void HalfCircleWaveGeneratorCallback(
    double& sample, unsigned short channel, std::size_t index, double phase,
    double amplitude, double phaseShift, double frequency)
{
    double xInt = 0.0;
    double x    = std::modf((phase + phaseShift)*2.0*frequency, &xInt)*2.0 - 1.0;
    double y    = std::sqrt(1.0 - x*x);

    if (static_cast<int>(xInt) % 2 == 1)
        y = -y;

    sample += y*amplitude;
}

AC_EXPORT SampleIterationFunction HalfCircleWaveGenerator(double amplitude, double phaseShift, double frequency)
{
    return std::bind(HalfCircleWaveGeneratorCallback, _1, _2, _3, _4, amplitude, phaseShift, frequency);
}

static void ReverseWaveGeneratorCallback(
    double& sample, unsigned short channel, std::size_t index, double phase,
    const WaveBuffer& buffer)
{
    sample = buffer.ReadSample(buffer.GetTotalTime() - phase, channel);
}

AC_EXPORT SampleIterationFunction ReverseWaveGenerator(const WaveBuffer& buffer)
{
    return std::bind(ReverseWaveGeneratorCallback, _1, _2, _3, _4, buffer);
}


} // /namesapce Synthesizer

} // /namespace Ac



// ================================================================================
