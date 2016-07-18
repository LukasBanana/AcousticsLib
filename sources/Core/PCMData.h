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
    std::int8_t*    bits8;
};

union PCMSampleConst
{
    const void*         raw;
    const std::int16_t* bits16;
    const std::int8_t*  bits8;
};

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
    const auto lowerEnd = static_cast<double>(std::numeric_limits<T>::lowest());
    const auto upperEnd = static_cast<double>(std::numeric_limits<T>::max());
    
    sample *= upperEnd;
    sample = std::max(lowerEnd, std::min(sample, upperEnd));
    
    data = static_cast<T>(sample);
}


} // /namespace Ac


#endif



// ================================================================================
