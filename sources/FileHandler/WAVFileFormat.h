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


//! RIFF WAVE tag: MS PCM
static const std::uint16_t tagRIFF_MS_PCM = 0x0001;


#include <Ac/PackPush.h>

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
