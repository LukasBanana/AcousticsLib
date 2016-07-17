/*
 * OGGStream.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifdef AC_PLUGIN_OGGVORBIS

#include "OGGStream.h"


namespace Ac
{


OGGStream::OGGStream(std::istream& stream) :
    stream_( stream )
{
}

OGGStream::~OGGStream()
{
}

std::size_t OGGStream::StreamWaveBuffer(WaveBuffer& buffer)
{
    return 0;
}


} // /namespace Ac


#endif



// ================================================================================
