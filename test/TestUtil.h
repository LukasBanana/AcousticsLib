/*
 * TestUtil.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AcLib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <cmath>
#include <array>
#include <iomanip>


template <typename T>
static std::string ToStr(T x, unsigned char decimals = 1)
{
    long long factor = 1;
    while (decimals-- > 0)
        factor *= 10;
    auto value = static_cast<long long>(x*factor);
    return std::to_string(value / factor) + '.' + std::to_string(value % factor);
}

static std::string GetTimeline(double time, double totalTime, std::size_t len = 40)
{
    std::size_t lenFront = static_cast<std::size_t>((time/totalTime)*static_cast<float>(len));
    std::size_t lenBack = len - lenFront;
    return '[' + std::string(lenFront, '_') + '|' + std::string(lenBack, '_') + ']';
}

static std::string GetTimeline(const Ac::Sound& snd, std::size_t len = 40)
{
    return GetTimeline(snd.GetSeek(), snd.TotalTime());
}


