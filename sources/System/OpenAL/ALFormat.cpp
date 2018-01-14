/*
 * ALFormat.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALFormat.h"


namespace Ac
{


bool ALFormatFromWaveFormat(ALenum& outFormat, const WaveBufferFormat& inFormat)
{
    if (inFormat.bitsPerSample == 16)
    {
        if (inFormat.channels == 2)
            outFormat = AL_FORMAT_STEREO16;
        else if (inFormat.channels == 1)
            outFormat = AL_FORMAT_MONO16;
        else
            return false;
    }
    else if (inFormat.bitsPerSample == 8)
    {
        if (inFormat.channels == 2)
            outFormat = AL_FORMAT_STEREO8;
        else if (inFormat.channels == 1)
            outFormat = AL_FORMAT_MONO8;
        else
            return false;
    }
    else
        return false;
    return true;
}

void WaveFormatFromALFormat(ALenum format, std::uint16_t& channels, std::uint16_t& bitsPerSample)
{
    switch (format)
    {
        case AL_FORMAT_MONO8:
            channels        = 1;
            bitsPerSample   = 8;
            break;
        case AL_FORMAT_MONO16:
            channels        = 1;
            bitsPerSample   = 16;
            break;
        case AL_FORMAT_STEREO8:
            channels        = 2;
            bitsPerSample   = 8;
            break;
        case AL_FORMAT_STEREO16:
            channels        = 2;
            bitsPerSample   = 16;
            break;
    }
}

std::string ALErrorToString(ALenum error)
{
    switch (error)
    {
        case AL_NO_ERROR:
            return "no error";
        case AL_INVALID_NAME:
            return "invalid name";
        case AL_INVALID_ENUM:
            return "invalid enumeration";
        case AL_INVALID_VALUE:
            return "invalid value";
        case AL_INVALID_OPERATION:
            return "invalid operation";
        case AL_OUT_OF_MEMORY:
            return "out of memory";
    }
    return "";
}


} // /namespace Ac



// ================================================================================
