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

    
static const unsigned int sampleRate44kHz = 44100;
static const unsigned int sampleRate8kHz = 8000;

/**
\brief Initializes the specified format with a PCM wave format.
\param[in] channels Specifies the number of channels.
This is commonly 1 (for mono) or 2 (for stereo) sounds.
\param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
\param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
\return The generated wave format. All members are initialized with
the respective values depending on the input parameters.
\see WaveFormat
\see WaveFormatTags::PCM
*/
AC_EXPORT void InitWaveBufferFormat(
    WaveFormat& format,
    unsigned short channels = 1,
    unsigned int sampleRate = sampleRate44kHz,
    unsigned short bitsPerSample = 16
);

AC_EXPORT void InitWaveBuffer(
    WaveBuffer& buffer,
    double duration,
    unsigned short channels = 1,
    unsigned int sampleRate = sampleRate44kHz,
    unsigned short bitsPerSample = 16
);

AC_EXPORT void GenerateWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, const std::function<double(double phase)>& waveFunction);

AC_EXPORT void GenerateSineWave(WaveBuffer& buffer, double phaseBegin, double phaseEnd, double amplitude, double phaseShift, double frequency);
    
    
} // /namesapce Synthesizer

} // /namespace Ac


#endif



// ================================================================================
