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


/* ----- Constants ----- */

//! 44.1 kHz sample rate.
static const unsigned int sampleRate44kHz   = 44100;

//! 22.05 kHz sample rate.
static const unsigned int sampleRate22kHz   = 22050;

//! 11.025 kHz sample rate.
static const unsigned int sampleRate11kHz   = 11025;

//! 8 kHz sample rate.
static const unsigned int sampleRate8kHz    = 8000;

/* ----- Wave generators ----- */

/**
\brief Returns a function object of a sine wave generator of the form: sin((phase + phaseShift)*2*PI*frequency)*amplitude.
\param[in] amplitude Specifies the sine wave amplitude (maximal sine value).
\param[in] phaseShift Specifies the phase shift value within the sine function.
\param[in] frequency Specifies the sine wave frequency. This frequency should be in the human hearable frequency range, which is 20 to 20,000 Hz.
\see WaveGeneratorFunction
\see GenerateWave
*/
AC_EXPORT SampleIterationFunction SineWaveGenerator(double amplitude, double phaseShift, double frequency);

AC_EXPORT SampleIterationFunction HalfCircleWaveGenerator(double amplitude, double phaseShift, double frequency);

AC_EXPORT SampleIterationFunction GaussianBlurWaveModifier(double variance = 1.0, std::size_t sampleSpread = 10);

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
    
    
} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
