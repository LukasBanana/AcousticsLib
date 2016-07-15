/*
 * WaveFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_FORMAT_H__
#define __AC_WAVE_FORMAT_H__


namespace Ac
{


#include "PackPush.h"

//! Audio wave buffer format structure.
struct WaveFormat
{
    unsigned short  formatTag;      //!< Encoding format tag. \see WaveFormatTags
    unsigned short  channels;       //!< Number of channels. 1 for mono and 2 for stereo.
    unsigned long   sampleRate;     //!< Number of samples per second (in Herz). The commonly used sample rates are: 8.0kHz, 11.025kHz, 22.05kHz and 44.1kHz.
    unsigned long   bytesPerSecond; //!< Number of bytes per second. Must be equal to (sampleRate * blockAlign).
    unsigned short  blockAlign;     //!< Channel block alignment. Must be equal to ((channels * bitsPerSample) / 8).
    unsigned short  bitsPerSample;  //!< Number of bits per sample.
}
AC_PACK_STRUCT;

#include "PackPop.h"


/**
\brief Generates a PCM wave format.
\param[in] channels Specifies the number of channels.
This is commonly 1 (for mono) or 2 (for stereo) sounds.
\param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
\param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
\return The generated wave format. All members are initialized with
the respective values depending on the input parameters.
\see WaveFormat
\see WaveFormatTags::PCM
*/
WaveFormat MakeWaveFormatPCM(unsigned short channels, unsigned long sampleRate, unsigned short bitsPerSample);


} // /namespace Ac


#endif



// ================================================================================
