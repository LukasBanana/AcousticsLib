/*
 * AIFFFileFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AIFF_FILE_FORMAT_H__
#define __AC_AIFF_FILE_FORMAT_H__


#include "FormatAuxiliary.h"


namespace Ac
{
    
    
// Version 1 of AIFF-C
#define AC_AIFC_VERSION_1 0xA2805140
    
    
using Extended80Bit = std::int8_t[10];

    
#include <Ac/PackPush.h>

struct AIFFHeader
{
    std::uint32_t   id;        //!< Format header ID (always 'FORM').
    std::int32_t    size;      //!< Data size of the 'FORM' chunk (in bytes).
    std::uint32_t   formType;  //!< Form type (either 'AIFF' for AIFF files, or 'AIFC' for AIFF-C files).
}
AC_PACK_STRUCT;

struct AIFFChunk
{
    std::uint32_t   id;   //!< Chunk ID.
    std::int32_t    size; //!< Chunk size (in bytes).
}
AC_PACK_STRUCT;

struct AIFCFormatVersionChunk
{
  /*std::uint32_t   id;         //!< Chunk ID must be 'FVER'. */
  /*std::int32_t    size;       //!< Chunk size (in bytes) must be 4. */
    std::uint32_t   timeStamp;  //!< Must be AC_AIFC_VERSION_1
}
AC_PACK_STRUCT;

struct AIFFCommonChunk
{
  /*std::uint32_t   id;             //!< Chunk ID must be 'COMM'. */
  /*std::int32_t    size;           //!< Chunk size (in bytes) must be 18 for AIFF or 22+ for AIFC. */
    std::int16_t    channels;       //!< Number of audio channels.
    std::uint32_t   sampleFrames;   //!< Sample frames = samples / channel.
    std::int16_t    bitsPerSample;  //!< Bits per sample (e.g. 8, 16).
    Extended80Bit   sampleRate;     //!< Sample frames / second.
}
AC_PACK_STRUCT;

struct AIFCCommonChunk
{
  /*std::uint32_t   id;                 //!< Chunk ID must be 'COMM'. */
  /*std::int32_t    size;               //!< Chunk size (in bytes) is variable. */
  /*std::int16_t    channels;           //!< Number of audio channels. */
  /*std::uint32_t   sampleFrames;       //!< Sample frames = samples / channel. */
  /*std::int16_t    bitsPerSample;      //!< Bits per sample (e.g. 8, 16). */
  /*Extended80Bit   sampleRate;         //!< Sample frames / second. */
    std::uint32_t   compressionType;    //!< Compression type ID code.
  /*pstring         compressionName;    //!< Human-readable compression type name (in Pascal-style string format). */
}
AC_PACK_STRUCT;

struct AIFFSoundChunk
{
  /*std::uint32_t   id;         //!< Chunk ID must be 'SSND'. */
  /*std::int32_t    size;       //!< Chunk size (in bytes) is variable. */
    std::uint32_t   offset;     //!< Determines where the first sample frame in the sound data starts.
    std::uint32_t   blockSize;  //!< Block size is used in conjunction with the offset for block-algining sound data.
}
AC_PACK_STRUCT;

#include <Ac/PackPop.h>


} // /namespace Ac


#endif



// ================================================================================
