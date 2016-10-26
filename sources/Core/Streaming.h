/*
 * Streaming.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_STREAMING_H
#define AC_STREAMING_H


#include <Ac/Export.h>
#include <Ac/Sound.h>
#include <Ac/WaveBuffer.h>


namespace Ac
{


AC_EXPORT void InitStreaming(Sound& sound, double startTime = 0.0, std::size_t queueAdvanceSize = 5);

AC_EXPORT void Streaming(Sound& sound, WaveBuffer& waveBuffer);

AC_EXPORT void Streaming(Sound& sound);


} // /namespace Ac


#endif



// ================================================================================
