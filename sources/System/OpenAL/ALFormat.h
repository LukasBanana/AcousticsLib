/*
 * ALFormat.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_FORMAT_H
#define AC_AL_FORMAT_H


#include "OpenAL.h"

#include <Ac/WaveBuffer.h>
#include <string>


namespace Ac
{


bool ALFormatFromWaveFormat(ALenum& outFormat, const WaveBufferFormat& inFormat);

void WaveFormatFromALFormat(ALenum format, std::uint16_t& channels, std::uint16_t& bitsPerSample);

std::string ALErrorToString(ALenum error);


} // /namespace Ac


#endif



// ================================================================================
