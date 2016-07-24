/*
 * Synthesizer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_SYNTHESIZER_H__
#define __AC_SYNTHESIZER_H__


#include "Export.h"
#include "WaveBuffer.h"
#include "MusicalNotes.h"

#include <functional>


namespace Ac
{

namespace Synthesizer
{


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

AC_EXPORT SampleIterationFunction NoiseGenerator(double amplitude);

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
    
    
} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
