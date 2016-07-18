/*
 * WaveFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_FORMAT_H__
#define __AC_WAVE_FORMAT_H__


#include <cstdint>


namespace Ac
{


#include "PackPush.h"

//! Audio wave buffer format structure.
struct WaveFormat
{
    std::uint16_t formatTag;        //!< Encoding format tag (see WaveFormatTags).
    std::uint16_t channels;         //!< Number of channels. 1 for mono and 2 for stereo.
    std::uint32_t sampleRate;       //!< Number of samples per second (in Herz). The commonly used sample rates are: 8.0kHz, 11.025kHz, 22.05kHz and 44.1kHz.
    std::uint32_t bytesPerSecond;   //!< Number of bytes per second. Must be equal to (sampleRate * blockAlign).
    std::uint16_t blockAlign;       //!< Channel block alignment. Must be equal to ((channels * bitsPerSample) / 8).
    std::uint16_t bitsPerSample;    //!< Number of bits per sample.
}
AC_PACK_STRUCT;

#include "PackPop.h"


} // /namespace Ac


#endif



// ================================================================================
