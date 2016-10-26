/*
 * MODFileFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_MOD_FILE_FORMAT_H
#define AC_MOD_FILE_FORMAT_H


#include <cstdint>


namespace Ac
{


// see http://www.fileformat.info/format/mod/corion.htm

#include <Ac/PackPush.h>

struct MODRecord
{
    char            name[22];
    std::uint16_t   length;
    std::uint8_t    finetune;
    std::uint8_t    volume;     //!< Sample volume (0 - 64)
    std::uint16_t   loopStart;
    std::uint16_t   loopLength;
}
AC_PACK_STRUCT;

struct MODHeader
{
    char            title[20];
    MODRecord       records[31];
    std::uint8_t    songLength;             //!< Song length in patterns (0 - 128)
    std::uint8_t    restart;                //!< Restart byte for song looping
    std::uint8_t    patternSequences[128];
    char            id[4];                  //!< "M.K.", "4CHN", "6CHN", "8CHN", "4FLT", "8FLT"
}
AC_PACK_STRUCT;

#include <Ac/PackPop.h>


} // /namespace Ac


#endif



// ================================================================================
