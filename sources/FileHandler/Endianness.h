/*
 * Endianness.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_ENDIANNESS_H__
#define __AC_ENDIANNESS_H__


#include <climits>


namespace Ac
{


template <typename T>
T SwapEndian(T value)
{
    static_assert(std::is_integral<T>::value, "SwapEndian template only allows integral types");

    union
    {
        T               value;
        unsigned char   bytes[sizeof(T)];
    }
    source, dest;

    source.value = value;

    for (std::size_t i = 0; i < sizeof(T); ++i)
        dest.bytes[i] = source.bytes[sizeof(T) - i - 1u];

    return dest.value;
}


} // /namespace Ac


#endif



// ================================================================================
