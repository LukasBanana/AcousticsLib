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
    

std::string GetStrinFromUINT32(std::uint32_t i);

template <typename T>
void Read(std::istream& stream, T& buffer)
{
    stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
}

void Ignore(std::istream& stream, std::streampos count);

//! Converts the specified 80-bit IEEE 754 extended precision floating-point into a 64-bit double precision floating point.
double ReadFloat80(const std::int8_t (&value)[10]);


} // /namespace Ac


#endif



// ================================================================================
