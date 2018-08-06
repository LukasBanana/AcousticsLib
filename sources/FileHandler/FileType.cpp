/*
 * FileType.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileType.h"
#include "FormatAuxiliary.h"
#include "MODFileFormat.h"
#include <vector>
#include <string>


namespace Ac
{


AudioFormats DetermineAudioFormat(std::istream& stream)
{
    /* Initialize with unknown format and set reading position to the beginning */
    auto format = AudioFormats::Unknown;

    if (stream.good())
    {
        const auto pos = stream.tellg();
        stream.seekg(0, std::ios_base::beg);

        /* Read magic number */
        std::uint32_t magicNumber = 0, formatType = 0;
        Read(stream, magicNumber);

        /* Check for RIFF WAVE */
        if (magicNumber == UINT32_FROM_STRING("RIFF"))
        {
            Ignore(stream, 4);
            Read(stream, formatType);

            if (formatType == UINT32_FROM_STRING("WAVE"))
                format = AudioFormats::WAVE;
        }

        /* Check for AIFF/AIFF-C */
        else if (magicNumber == UINT32_FROM_STRING("FORM"))
        {
            Ignore(stream, 4);
            Read(stream, formatType);

            if (formatType == UINT32_FROM_STRING("AIFF"))
                format = AudioFormats::AIFF;
            else if (formatType == UINT32_FROM_STRING("AIFC"))
                format = AudioFormats::AIFC;
        }

        /* Check for Ogg-Vorbis */
        else if (magicNumber == UINT32_FROM_STRING("OggS"))
        {
            format = AudioFormats::OggVorbis;
        }

        /* Check for Amiga Module */
        else
        {
            Ignore(stream, sizeof(MODHeader) - 8);
            Read(stream, magicNumber);

            for (const auto& s : std::vector<std::string>{ "M.K.", "4CHN", "6CHN", "8CHN", "4FLT", "8FLT" })
            {
                if (magicNumber == UINT32_FROM_STRING(s.c_str()))
                {
                    format = AudioFormats::AmigaModule;
                    break;
                }
            }
        }

        /* Reset reading position and return determined format */
        stream.seekg(pos, std::ios_base::beg);
    }

    return format;
}


} // /namespace Ac



// ================================================================================
