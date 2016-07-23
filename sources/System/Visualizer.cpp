/*
 * Visualizer.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

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
    Renderer& renderer, const WaveBuffer& buffer, unsigned short channel, const Gs::Vector2ui& size)//, const WaveBufferRange& range)
{
    if (channel >= buffer.GetFormat().channels)
        return;
    
    // Generate all lines to draw for the horizontal axis
    std::vector<Gs::Vector2i> verts(size.x * 2u, Gs::Vector2i());
    
    double time         = 0.0;
    double timeStep     = buffer.GetTotalTime() / static_cast<double>(size.x);
    double halfHeight   = static_cast<double>(size.y)*0.5;
    
    for (std::size_t x = 0; x < size.x; ++x)
    {
        auto& a = verts[x*2];
        auto& b = verts[x*2 + 1];
        
        /* Get amplitude range for the current time window */
        double ampMin = 0.0, ampMax = 0.0;
        GetAmplitudeRange(buffer, channel, time, time + timeStep, ampMin, ampMax);
        
        /* Setup line vertices */
        a.x = b.x = static_cast<int>(x);
        
        a.y = halfHeight + halfHeight*ampMin;
        b.y = halfHeight + halfHeight*ampMax;
    }
    
    // Draw wave buffer
    renderer.BeginDrawing(size);
    renderer.DrawLineList(verts);
    renderer.EndDrawing();
}


} // /namesapce Visualizer
    
} // /namespace Ac



// ================================================================================
