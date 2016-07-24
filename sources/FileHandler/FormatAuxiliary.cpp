/*
 * FormatAuxiliary.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FormatAuxiliary.h"


namespace Ac
{


// see http://stackoverflow.com/questions/2963055/msvc-win32-convert-extended-precision-float-80-bit-to-double-64-bit
double ReadFloat80(const std::int8_t (&value)[10])
{
    int exponent = (((value[9] << 8) | value[8]) & 0x7FFF);

    std::uint64_t mantissa = (
        ((std::uint64_t)value[7] << 56) |
        ((std::uint64_t)value[6] << 48) |
        ((std::uint64_t)value[5] << 40) |
        ((std::uint64_t)value[4] << 32) | 
        ((std::uint64_t)value[3] << 24) |
        ((std::uint64_t)value[2] << 16) |
        ((std::uint64_t)value[1] <<  8) |
         (std::uint64_t)value[0]
    );

    union
    {
        unsigned char d[8] = { 0 };
        double result;
    };

    /* Set sign */
    d[7] = (value[9] & 0x80);

    if (exponent == 0x7FFF || exponent == 0)
    {
        /* Infinite, NaN or denormal */
        if (exponent == 0x7FFF)
        {
            /* Infinite or NaN */
            d[7] |= 0x7F;
            d[6]  = 0xF0;
        }
        else
        {
            /* Otherwise it's denormal; It cannot be represented as double; Translate as singed zero */
            return result;
        }
    }
    else
    {
        /* Normal number; exponent for double precision */
        exponent = exponent - 0x3FFF + 0x03FF;

        if (exponent <= -52)
        {
            /* Too small to represent; Translate as (signed) zero */
            return result;
        }
        else if (exponent < 0)
        {
            /* Denormal, exponent bits are already zero here */
        }
        else if (exponent >= 0x07FF)
        {
            /* Too large to represent; Translate as infinite */
            d[7] |= 0x7F;
            d[6]  = 0xF0;
            for (int i = 0; i < 6; ++i)
                d[i] = 0;
            return result;
        }
        else
        {
            /* Representable number */
            d[7] |= (exponent & 0x07F0) >> 4;
            d[6] |= (exponent & 0x000F) << 4;
        }
    }
    
    /* Translate mantissa */
    mantissa >>= 11;

    if (exponent < 0)
    {
        /* Denormal, further shifting is required here */
        mantissa >>= (-exponent + 1);
    }

    d[0]  =  mantissa        & 0xFF;
    d[1]  = (mantissa >>  8) & 0xFF;
    d[2]  = (mantissa >> 16) & 0xFF;
    d[3]  = (mantissa >> 24) & 0xFF;
    d[4]  = (mantissa >> 32) & 0xFF;
    d[5]  = (mantissa >> 40) & 0xFF;
    d[6] |= (mantissa >> 48) & 0x0F;

    return result;
}


} // /namespace Ac



// ================================================================================
