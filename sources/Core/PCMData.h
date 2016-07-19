/*
 * PCMData.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_PCM_DATA_H__
#define __AC_PCM_DATA_H__


#include <cstdint>
#include <limits>
#include <algorithm>


namespace Ac
{


union PCMSample
{
    void*           raw;
    std::int16_t*   bits16;
    std::uint8_t*   bits8;
};

union PCMSampleConst
{
    const void*         raw;
    const std::int16_t* bits16;
    const std::uint8_t* bits8;
};

struct PCMLimits
{
    double lowerEnd;
    double upperEnd;
    double range;
    double nullPoint;
};

template <typename T>
inline PCMLimits GetPCMLimits()
{
    PCMLimits limits;

    limits.lowerEnd     = static_cast<double>(std::numeric_limits<T>::lowest());
    limits.upperEnd     = static_cast<double>(std::numeric_limits<T>::max());
    
    limits.range        = limits.upperEnd - limits.lowerEnd;
    limits.nullPoint    = 0.5*(limits.lowerEnd + limits.upperEnd);

    return limits;
}

template <typename T>
inline void PCMDataToSample(double& sample, const T data)
{
    /* Scale sample and clamp into range [min, max] */
    sample = static_cast<double>(data);
    
    const auto upperEnd = static_cast<double>(std::numeric_limits<T>::max());
    sample /= upperEnd;
}

template <typename T>
inline void SampleToPCMData(T& data, double sample)
{
    /* Scale sample and clamp into range [min, max] */
    const auto limtis = GetPCMLimits<T>();

    sample = sample * limtis.range*0.5 + limtis.nullPoint;
    sample = std::max(limtis.lowerEnd, std::min(sample, limtis.upperEnd));
    
    data = static_cast<T>(sample);
}


} // /namespace Ac


#endif



// ================================================================================
