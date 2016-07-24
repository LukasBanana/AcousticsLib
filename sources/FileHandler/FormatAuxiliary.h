/*
 * FormatAuxiliary.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_FORMAT_AUXILIARY_H__
#define __AC_FORMAT_AUXILIARY_H__


#include <cstdint>
#include <climits>
#include <istream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>


namespace Ac
{


#define UINT32_FROM_STRING(s) (*reinterpret_cast<const std::uint32_t*>(s))
    

inline std::string GetStrinFromUINT32(std::uint32_t i)
{
    std::string s(4, ' ');
    *reinterpret_cast<std::uint32_t*>(&s[0]) = i;
    return s;
}

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

template <typename T>
static void Read(std::istream& stream, T& buffer)
{
    stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
}

//! Converts the specified 80-bit IEEE 754 extended precision floating-point into a 64-bit double precision floating point.
double ReadFloat80(const std::int8_t (&value)[10]);


} // /namespace Ac


#endif



// ================================================================================
