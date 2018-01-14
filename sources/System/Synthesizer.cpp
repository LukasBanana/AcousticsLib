/*
 * Synthesizer.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Core/PCMData.h"

#include <Ac/Synthesizer.h>
#include <Gauss/Algebra.h>
#include <cmath>


using namespace std::placeholders;

namespace Ac
{

namespace Synthesizer
{


/* ----- Wave generators ----- */

static void SineGeneratorCallback(
    double& sample, std::uint16_t channel, std::size_t index, double timePoint,
    double amplitude, double phase, double frequency)
{
    sample += std::sin((timePoint + phase)*2.0*M_PI*frequency)*amplitude;
}

AC_EXPORT SampleIterationFunction SineGenerator(double amplitude, double phase, double frequency)
{
    return std::bind(SineGeneratorCallback, _1, _2, _3, _4, amplitude, phase, frequency);
}

static void HalfCircleGeneratorCallback(
    double& sample, std::uint16_t channel, std::size_t index, double timePoint,
    double amplitude, double phase, double frequency)
{
    double xInt = 0.0;
    double x    = std::modf((timePoint + phase)*2.0*frequency, &xInt)*2.0 - 1.0;
    double y    = std::sqrt(1.0 - x*x);

    if (static_cast<int>(xInt) % 2 == 1)
        y = -y;

    sample += y*amplitude;
}

AC_EXPORT SampleIterationFunction HalfCircleGenerator(double amplitude, double phase, double frequency)
{
    return std::bind(HalfCircleGeneratorCallback, _1, _2, _3, _4, amplitude, phase, frequency);
}

AC_EXPORT SampleIterationFunction Amplifier(double multiplicator)
{
    return [multiplicator](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
    {
        sample *= multiplicator;
    };
}

static double Random()
{
    return (static_cast<double>(rand()) / RAND_MAX);
}

static double Random(double a, double b)
{
    return (a + (b - a) * Random());
}

AC_EXPORT SampleIterationFunction WhiteNoiseGenerator(double amplitude)
{
    return [amplitude](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
    {
        sample += Random(-1.0, 1.0) * amplitude;
    };
}

AC_EXPORT SampleIterationFunction BrownNoiseGenerator(double amplitude, double& state)
{
    state = 0.0;
    return [amplitude, &state](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
    {
        auto noiseLerp = (state + 1.0)*0.5;
        auto noise = Random(
            Gs::Lerp(0.0, -amplitude, noiseLerp),
            Gs::Lerp(amplitude, 0.0, noiseLerp)
        );
        state += noise;
        sample = state;
    };
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
        [&bufferCopy](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
        {
            sample = bufferCopy.ReadSample(bufferCopy.GetSampleFrames() - index - 1u, channel);
        }
    );
}

static double NormalDistribution(double x, double mean, double variance)
{
    return std::exp(-(x - mean)*(x - mean) / (2.0 * variance)) / std::sqrt(2.0 * M_PI * variance);
}

AC_EXPORT void BlurWaveBuffer(WaveBuffer& buffer, double timeSpread, double variance, std::size_t sampleCount)
{
    /* Compute weights for the normal distribution */
    static const double maxDistributionSpread = 7.0;

    std::vector<double> weights(sampleCount + 1u, 0.0);
    double weightSum = 0.0;

    for (std::size_t i = 0; i <= sampleCount; ++i)
    {
        auto x = (static_cast<double>(i) / sampleCount - 0.5) * maxDistributionSpread;
        weights[i] = NormalDistribution(x, 0.0, variance);
        weightSum += weights[i];
    }

    /* Normalize weights */
    weightSum = 1.0 / weightSum;
    for (auto& weight : weights)
        weight *= weightSum;

    /* Read from the copy of the buffer to prevent reading and writing on the same buffer */
    auto bufferCopy = buffer;
    buffer.ForEachSample(
        [&](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
        {
            sample = 0.0;

            for (std::size_t i = 0; i <= sampleCount; ++i)
            {
                auto timeOffset = timeSpread * (static_cast<double>(i) / sampleCount - 0.5);
                sample += bufferCopy.ReadSample(timePoint + timeOffset, channel) * weights[i];
            }
        }
    );
}

AC_EXPORT void FadeWaveBuffers(
    WaveBuffer& buffer, const WaveBuffer& bufferFadeFrom, const WaveBuffer& bufferFadeTo,
    double timePointFrom, double timePointTo, const FadingFunction& fading, bool writeOutlines)
{
    /* Quit if operation has no effect */
    if ((&buffer) == (&bufferFadeFrom) && (&buffer) == (&bufferFadeTo))
        return;

    /* Clamp input parameters */
    timePointFrom = Gs::Clamp(timePointFrom, 0.0, buffer.GetTotalTime());
    timePointTo = Gs::Clamp(timePointTo, timePointFrom, buffer.GetTotalTime());

    /* Interpolate between the two fading buffers */
    if (timePointFrom < timePointTo)
    {
        buffer.ForEachSample(
            [&](double& sample, std::uint16_t channel, std::size_t index, double timePoint)
            {
                /* Compute fading interpolator */
                double t = (timePoint - timePointFrom) / (timePointTo - timePointFrom);
                if (fading)
                    fading(t);

                /* Fade between two buffer samples */
                sample = Gs::Lerp(
                    bufferFadeFrom.ReadSample(timePoint, channel),
                    bufferFadeTo.ReadSample(timePoint, channel),
                    t
                );
            },
            timePointFrom,
            timePointTo
        );
    }

    /* Write outlines (if enabled) */
    if (writeOutlines)
    {
        /* Write left outlines (if buffers are not identical) */
        if ((&buffer) != (&bufferFadeFrom))
            buffer.CopyFrom(bufferFadeFrom, 0.0, timePointFrom, 0.0);

        /* Write right outlines (if buffers are not identical) */
        if ((&buffer) != (&bufferFadeTo))
            buffer.CopyFrom(bufferFadeTo, timePointTo, buffer.GetTotalTime(), timePointTo);
    }
}


} // /namesapce Synthesizer

} // /namespace Ac



// ================================================================================
