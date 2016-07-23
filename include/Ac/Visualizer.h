/*
 * Visualizer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_VISUALIZER_H__
#define __AC_VISUALIZER_H__


#include "Export.h"
#include "WaveBuffer.h"
#include "Renderer.h"


namespace Ac
{

namespace Visualizer
{


AC_EXPORT void DrawWaveBuffer(
    Renderer& renderer,
    const WaveBuffer& buffer,
    unsigned short channel,
    const Gs::Vector2i& position,
    const Gs::Vector2i& size,
    bool smoothWave = false
);

AC_EXPORT void DrawWaveBuffer(
    Renderer& renderer,
    const WaveBuffer& buffer,
    unsigned short channel,
    const Gs::Vector2i& position,
    const Gs::Vector2i& size,
    double timeBegin,
    double timeEnd,
    bool smoothWave = false
);
    
    
} // /namesapce Visualizer

} // /namespace Ac


#endif



// ================================================================================
