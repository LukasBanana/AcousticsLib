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


/* ----- Wave generators ----- */

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

/* ----- Misc ----- */

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

AC_EXPORT void ReverseWaveBuffer(WaveBuffer& buffer)
{
    /* Read from the copy of the buffer to prevent reading and writing on the same buffer */
    auto bufferCopy = buffer;
    buffer.ForEachSample(
        [&bufferCopy](double& sample, unsigned short channel, std::size_t index, double phase)
        {
            sample = bufferCopy.ReadSample(bufferCopy.GetSampleCount() - index - 1u, channel);
        }
    );
}


} // /namesapce Synthesizer

} // /namespace Ac



// ================================================================================
