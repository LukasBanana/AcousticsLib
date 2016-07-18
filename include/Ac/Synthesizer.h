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
using WaveGenerationFunction = std::function<void(double& sample, unsigned short channel, double phase)>;
    
//! 44.1 kHz sample rate.
static const unsigned int sampleRate44kHz   = 44100;
//! 8 kHz sample rate.
static const unsigned int sampleRate8kHz    = 8000;

/**
\brief Initializes the specified format with a PCM wave format.
\param[out] format Specifies the resulting wave format.
\param[in] channels Specifies the number of channels. This is commonly 1 (for mono) or 2 (for stereo) sounds.
\param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
\param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
\see WaveFormat
\see WaveFormatTags::PCM
*/
AC_EXPORT void InitWaveBufferFormat(
    WaveFormat& format,
    unsigned short channels = 1,
    unsigned int sampleRate = sampleRate44kHz,
    unsigned short bitsPerSample = 16
);

/**
\brief Initializes the specified wave buffer with a PCM wave format.
\param[out] buffer Specifies the resulting wave buffer.
\param[in] channels Specifies the number of channels. This is commonly 1 (for mono) or 2 (for stereo) sounds.
\param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
\param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
\see WaveFormat
\see WaveFormatTags::PCM
*/
AC_EXPORT void InitWaveBuffer(
    WaveBuffer& buffer,
    double duration,
    unsigned short channels = 1,
    unsigned int sampleRate = sampleRate44kHz,
    unsigned short bitsPerSample = 16
);

/**
\brief Main function for wave buffer generation.
\param[in,out] buffer Specifies the buffer which is to be modified.
\param[in] phaseBegin Specifies the phase beginning (in seconds).
\param[in] phaseEnd Specifies the phase ending (in seconds).
\param[in] waveFunction Specifies the generation callback function.
\see WaveGenerationFunction
*/
AC_EXPORT void GenerateWave(
    WaveBuffer& buffer,
    double phaseBegin,
    double phaseEnd,
    const WaveGenerationFunction& waveFunction
);

AC_EXPORT void GenerateSineWave(
    WaveBuffer& buffer,
    double phaseBegin,
    double phaseEnd,
    double amplitude,
    double phaseShift,
    double frequency
);
    
    
} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
