/*
 * Visualizer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_VISUALIZER_H
#define AC_VISUALIZER_H


#include "Export.h"
#include "WaveBuffer.h"
#include "Renderer.h"


namespace Ac
{

namespace Visualizer
{


/**
\brief Draws the specified wave buffer for within a given time window.
\param[in,out] renderer Specifies the renderer which is used to draw the audio signal.
This must be an instance of a class which implements the "Renderer" interface.
\param[in] channel Specifies which audio channel to draw.
\param[in] position Specifies the position where to start with drawing.
\param[in] size Specifies the size of the wave buffer on the render context.
\param[in] timeBegin Specifies the beginning of the wave buffer to draw (in seconds).
\param[in] timeEnd Specifies the end of the wave buffer to draw (in seconds).
\see Renderer
*/
AC_EXPORT void DrawWaveBuffer(
    Renderer&           renderer,
    const WaveBuffer&   buffer,
    std::uint16_t       channel,
    const Gs::Vector2i& position,
    const Gs::Vector2i& size,
    double              timeBegin,
    double              timeEnd
);

/**
\brief Draws the specified wave buffer entirely.
\see DrawWaveBuffer(Renderer&, const WaveBuffer&, std::uint16_t, const Gs::Vector2i&, const Gs::Vector2i&, double, double)
*/
AC_EXPORT void DrawWaveBuffer(
    Renderer&           renderer,
    const WaveBuffer&   buffer,
    std::uint16_t       channel,
    const Gs::Vector2i& position,
    const Gs::Vector2i& size
);


} // /namesapce Visualizer

} // /namespace Ac


#endif



// ================================================================================
