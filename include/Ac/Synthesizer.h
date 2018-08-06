/*
 * Synthesizer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_SYNTHESIZER_H
#define AC_SYNTHESIZER_H


#include "Export.h"
#include "WaveBuffer.h"
#include "MusicalNotes.h"

#include <functional>


namespace Ac
{

namespace Synthesizer
{


/**
\brief Fading function interface.
\param[in,out] t Specifies the interpolation value.
Either the input and output value must always be in the range [0, 1].
\see FadeWaveBuffers
*/
using FadingFunction = std::function<void(double& t)>;

/* ----- Wave generators ----- */

/**
\brief Returns a function object of a sine wave generator of the form: sin((timePoint + phase)*2*PI*frequency)*amplitude.
\param[in] amplitude Specifies the sine wave amplitude (maximal sine value).
\param[in] phase Specifies the phase value within the sine function.
\param[in] frequency Specifies the sine wave frequency. This frequency should be in the human hearable frequency range, which is 20 to 20,000 Hz.
\see WaveGeneratorFunction
\see GenerateWave
*/
AC_EXPORT SampleIterationFunction SineGenerator(double amplitude, double phase, double frequency);

AC_EXPORT SampleIterationFunction HalfCircleGenerator(double amplitude, double phase, double frequency);

AC_EXPORT SampleIterationFunction Amplifier(double multiplicator);

AC_EXPORT SampleIterationFunction WhiteNoiseGenerator(double amplitude);

/**
\brief Returns a function object of a "brown-noise" wave generator.
\param[in] amplitude Specifies the wave amplitude (maximal value in the range [-amplitude, amplitude]).
\param[out] state Specifies the noise generation state and is only used as temporary storage.
This can also be uninitialized since it will be set to 0.0 at start up time.
*/
AC_EXPORT SampleIterationFunction BrownNoiseGenerator(double amplitude, double& state);

/* ----- Misc ----- */

/**
\brief Returns the frequency (in Hz) for the specified musical note.
\param[in] note Specifies the musical note (base value).
\param[in] interval Specifies the interval of the musical note.
*/
AC_EXPORT double GetNoteFrequency(const MusicalNotes note, int interval);

/**
\brief Reverses the specified wave buffer.
\see ReverseWaveGenerator
*/
AC_EXPORT void ReverseWaveBuffer(WaveBuffer& buffer);

AC_EXPORT void BlurWaveBuffer(WaveBuffer& buffer, double timeSpread = 0.1, double variance = 1.0, std::size_t sampleCount = 6);

/**
\brief Fades (or rather interpolates) between the two constant wave buffers.
\param[in,out] buffer Specifies the buffer which is to be modified.
\param[in] bufferFadeFrom Specifies the buffer from which the fading is starting. This can also be the output buffer.
\param[in] bufferFadeTo Specifies the buffer to which the fading is ending. This can also be the output buffer.
\param[in] timePointFrom Specifies the time point from which the fading is starting. This will be clamped to [0, buffer.GetTotalTime()].
\param[in] timePointTo Specifies the time point to which the fading is ending. This will be clamped to [timePointFrom, buffer.GetTotalTime()].
\param[in] fading Specifies the fading modification function. If this is null, no fading modification is applied. By default null.
\param[in] writeOutlines Specifies wether the outline samples (i.e. outside the range [timePointForm, timePointEnd]) will also be written or not. By default true.
*/
AC_EXPORT void FadeWaveBuffers(
    WaveBuffer&             buffer,
    const WaveBuffer&       bufferFadeFrom,
    const WaveBuffer&       bufferFadeTo,
    double                  timePointFrom,
    double                  timePointTo,
    const FadingFunction&   fading = nullptr,
    bool                    writeOutlines = true
);


} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
