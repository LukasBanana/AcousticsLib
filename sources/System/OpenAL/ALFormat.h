/*
 * ALFormat.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_FORMAT_H__
#define __AC_AL_FORMAT_H__


#include "OpenAL.h"

#include <Ac/WaveBuffer.h>


namespace Ac
{


bool ALFormatFromWaveFormat(ALenum& outFormat, const WaveBufferFormat& inFormat);

void WaveFormatFromALFormat(ALenum format, unsigned short& channels, unsigned short& bitsPerSample);


} // /namespace Ac


#endif



// ================================================================================
