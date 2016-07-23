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

#if 0

//! Extract integral and fractional part from extended floating-point (N must be at least 1).
template <std::size_t N>
void GetIntAndFract(std::int8_t (&value)[N], long long& intPart, unsigned long long& fractPart, std::size_t expSize)
{
    static_assert(N >= 1, "GetIntAndFract does not allow floating-points with less than 8 bits");
    
    // Returns true if the bit in 'value' at the specified is 1
    auto IsBit = [&](std::size_t pos) -> bool
    {
        return ((value[pos / 8] & (0x80 >> (pos % 8))) != 0);
    };
    
    // Returns the bit mask of 'value' in the range [begin, end)
    auto GetBitMask = [&](std::size_t begin, std::size_t end) -> unsigned long long
    {
        unsigned long long mask = 0;
        
        for (; begin < end; ++begin)
        {
            if (IsBit(begin))
                mask |= static_cast<unsigned long long>(-1);
            mask >>= 1;
        }
        
        return mask;
    };
    
    /* Extract sign (from first bit), exponent, and mantissa */
    bool sign   = ((value[0] & 0x80) != 0);
    auto exp    = GetBitMask(1u, 1u + expSize);
    bool intBit = (GetBitMask(1u + expSize, 1u + expSize) != 0);
    auto mnt    = GetBitMask(2u + expSize, N*8);
    
    /* Construct integral and fractional part */
    intPart     = 0;
    fractPart   = 0;
    
    if (exp == 0)
    {
        if (intBit)
        {
            
        }
        else if (mnt != 0)
        {
            // Result is (-1)^sign * mnt * 2^(-16382)
            std::stringstream s;
            s << (intBit ? '1' : '0') << '.' << mnt;
            auto ss = s.str();
            double m = std::stod(s.str().c_str());
            double d = m * std::pow(2.0, -16382.0);
            double di = 0.0;
            double df = std::modf(d, &di);
            intPart = static_cast<long long>(di);
            fractPart = static_cast<unsigned long long>(df);
        }
    }
    else
    {
        
    }
    
    if (sign)
        intPart = -intPart;
}

#endif


} // /namespace Ac


#endif



// ================================================================================
