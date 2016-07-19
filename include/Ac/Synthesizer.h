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


/**
\brief Wave generation function interface.
\param[in,out] sample Specifies the current sample which is to be modified. Each sample will be clamped to the range [-1, 1].
\param[in] channel Specifies the current channel to which the sample belongs.
\param[in] phase Specifies the current phase within the entire wave buffer (in seconds).
\remarks This function interface is used for the 'GenerateWave' function.
\see GenerateWave
*/
using WaveGeneratorFunction = std::function<void(double& sample, unsigned short channel, double phase)>;
    
//! 44.1 kHz sample rate.
static const unsigned int sampleRate44kHz   = 44100;

//! 22.05 kHz sample rate.
static const unsigned int sampleRate22kHz   = 22050;

//! 11.025 kHz sample rate.
static const unsigned int sampleRate11kHz   = 11025;

//! 8 kHz sample rate.
static const unsigned int sampleRate8kHz    = 8000;

/**
\brief Returns the frequency (in Hz) for the specified musical note.
\param[in] note Specifies the musical note (base value).
\param[in] interval Specifies the interval of the musical note.
*/
AC_EXPORT double GetNoteFrequency(const MusicalNotes note, int interval);

/**
\brief Main function for wave buffer generation.
\param[in,out] buffer Specifies the buffer which is to be modified.
\param[in] phaseBegin Specifies the phase beginning (in seconds). This will be clamped to [0, +inf).
\param[in] phaseEnd Specifies the phase ending (in seconds). This will be clamped to [phaseBegin, +inf).
\param[in] waveGenerator Specifies the wave generator callback function.
\see WaveGeneratorFunction
*/
AC_EXPORT void GenerateWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, const WaveGeneratorFunction& waveGenerator);

/**
\brief Modifies the entire wave buffer.
\see GenerateWave(WaveBuffer&, double, double, const WaveGeneratorFunction&)
*/
AC_EXPORT void GenerateWave(WaveBuffer& buffer, const WaveGeneratorFunction& waveGenerator);

/**
\brief Returns a function object of a sine wave generator of the form: sin((phase + phaseShift)*2*PI*frequency)*amplitude.
\param[in] amplitude Specifies the sine wave amplitude (maximal sine value).
\param[in] phaseShift Specifies the phase shift value within the sine function.
\param[in] frequency Specifies the sine wave frequency. This frequency should be in the human hearable frequency range, which is 20 to 20,000 Hz.
\see WaveGeneratorFunction
\see GenerateWave
*/
AC_EXPORT WaveGeneratorFunction SineWaveGenerator(double amplitude, double phaseShift, double frequency);

AC_EXPORT WaveGeneratorFunction HalfCircleWaveGenerator(double amplitude, double phaseShift, double frequency);

/**
\brief Returns a function object of a reversion wave generator.
\param[in] buffer Specifies the buffer to reverse.
\remarks Note that the input buffer of this function should not be the same buffer object
which is currently being modified within the "GenerateWave" function!
\see WaveGeneratorFunction
\see GenerateWave
*/
AC_EXPORT WaveGeneratorFunction ReverseWaveGenerator(const WaveBuffer& buffer);
    
    
} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
