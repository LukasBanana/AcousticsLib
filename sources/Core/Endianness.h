/*
 * Endianness.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_ENDIANNESS_H__
#define __AC_ENDIANNESS_H__


#include <algorithm>
#include <cstdlib>


namespace Ac
{


//! Swap endianness of common type (both integral and floating-points).
template <typename T>
void SwapEndian(T& value)
{
    union
    {
        T               value;
        unsigned char   bytes[sizeof(T)];
    }
    source, dest;

    source.value = value;

    for (std::size_t i = 0; i < sizeof(T); ++i)
        dest.bytes[i] = source.bytes[sizeof(T) - i - 1u];

    value = dest.value;
}

//! Swap endianness of array type.
template <typename T, std::size_t N>
void SwapEndian(T (&value)[N])
{
    for (std::size_t i = 0; i < N/2; ++i)
        std::swap(value[i], value[N - i - 1u]);
}


} // /namespace Ac


#endif



// ================================================================================
