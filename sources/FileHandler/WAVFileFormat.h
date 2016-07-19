/*
 * WAVFileFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAV_FILE_FORMAT_H__
#define __AC_WAV_FILE_FORMAT_H__


#include <cstdint>


namespace Ac
{


#define UINT32_FROM_STRING(s) (*reinterpret_cast<const std::uint32_t*>(s))


#include <Ac/PackPush.h>

struct RIFFWAVEFormat
{
    std::uint16_t formatTag;        //!< Encoding format tag (see WaveFormatTags).
    std::uint16_t channels;         //!< Number of channels. 1 for mono and 2 for stereo.
    std::uint32_t sampleRate;       //!< Number of samples per second (in Herz). The commonly used sample rates are: 8 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz.
    std::uint32_t bytesPerSecond;   //!< Number of bytes per second. Must be equal to (sampleRate * blockAlign).
    std::uint16_t blockAlign;       //!< Channel block alignment. Must be equal to ((channels * bitsPerSample) / 8).
    std::uint16_t bitsPerSample;    //!< Number of bits per sample.
}
AC_PACK_STRUCT;

struct RIFFWAVEChunk
{
    std::uint32_t id;   //!< Chunk ID (either 'fmt ' or 'data').
    std::uint32_t size; //!< Chunk size (in bytes).
}
AC_PACK_STRUCT;

#include <Ac/PackPop.h>


} // /namespace Ac


#endif



// ================================================================================
