/*
 * WindowFunctions.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WINDOW_FUNCTIONS_H__
#define __AC_WINDOW_FUNCTIONS_H__


#include <cmath>
#include <cstdlib>


namespace Ac
{

namespace WindowFunctions
{


template <typename T>
T BlackmanWindow(std::size_t n, std::size_t N, const T& alpha = T(0.16))
{
    if (N > 1 && n < N)
    {
        const auto a0 = (T(1) - alpha) / T(2);
        const auto a1 = T(1) / T(2);
        const auto a2 = alpha / T(2);

        return a0 - a1 * std::cos((T(2)*M_PI*n) / (N - 1)) + a2 * std::cos((T(4)*M_PI*n) / (N - 1));
    }
    return T(0);
}

template <typename T>
T HannWindow(std::size_t n, std::size_t N)
{
    if (N > 1 && n < N)
        return (T(1) - std::cos((T(2)*M_PI*n) / (N - 1))) / T(2);
    return T(0);
}


} // /namespace WindowFunctions

} // /namespace Ac


#endif



// ================================================================================
