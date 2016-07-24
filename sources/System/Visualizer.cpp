/*
 * Visualizer.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Core/WindowFunctions.h"
#include <Ac/Visualizer.h>
#include <limits>


namespace Ac
{
    
namespace Visualizer
{


static void GetAmplitudeRange(
    const WaveBuffer& buffer, unsigned short channel, double timeBegin, double timeEnd, double& ampMin, double& ampMax)
{
    auto indexBegin = buffer.GetIndexFromTimePoint(timeBegin);
    auto indexEnd   = buffer.GetIndexFromTimePoint(timeEnd);
    
    if (indexBegin > indexEnd)
        std::swap(indexBegin, indexEnd);

    ampMin = std::numeric_limits<double>::max();
    ampMax = std::numeric_limits<double>::lowest();
    
    for (std::size_t i = indexBegin; i <= indexEnd; ++i)
    {
        auto sample = buffer.ReadSample(i, channel);
        ampMin = std::min(ampMin, sample);
        ampMax = std::max(ampMax, sample);
    }
}

AC_EXPORT void DrawWaveBuffer(
    Renderer& renderer, const WaveBuffer& buffer, unsigned short channel,
    const Gs::Vector2i& position, const Gs::Vector2i& size, double timeBegin, double timeEnd)
{
    if (channel >= buffer.GetFormat().channels || size.x <= 0 || size.y <= 0)
        return;
    
    // Generate all lines to draw for the horizontal axis
    std::vector<Gs::Vector2i> verts(size.x * 2u, Gs::Vector2i());
    
    double time         = timeBegin;
    double timeStep     = (timeEnd - timeBegin) / static_cast<double>(size.x);
    double halfHeight   = static_cast<double>(size.y)*0.5;
    
    for (decltype(size.x) x = 0; x < size.x; ++x)
    {
        auto& a = verts[x*2];
        auto& b = verts[x*2 + 1];
        
        /* Get amplitude range for the current time window */
        double ampMin = 0.0, ampMax = 0.0;
        GetAmplitudeRange(buffer, channel, time, time + timeStep, ampMin, ampMax);
        
        /* Setup line vertices */
        a.x = b.x = static_cast<int>(x) + position.x;
        
        a.y = static_cast<int>(halfHeight + halfHeight*ampMin) + position.y;
        b.y = static_cast<int>(halfHeight + halfHeight*ampMax) + position.y;
        
        if (b.y == a.y)
            ++b.y;
        
        time += timeStep;
    }
    
    // Draw wave buffer
    renderer.BeginDrawing(size);
    renderer.DrawLineList(verts);
    renderer.EndDrawing();
}

AC_EXPORT void DrawWaveBuffer(
    Renderer& renderer, const WaveBuffer& buffer, unsigned short channel, const Gs::Vector2i& position, const Gs::Vector2i& size)
{
    DrawWaveBuffer(renderer, buffer, channel, position, size, 0.0, buffer.GetTotalTime());
}


} // /namesapce Visualizer
    
} // /namespace Ac



// ================================================================================
