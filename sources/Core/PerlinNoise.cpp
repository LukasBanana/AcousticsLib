/*
 * PerlinNoise.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/PerlinNoise.h>
#include <random>
#include <algorithm>
#include <limits.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>


namespace Ac
{


PerlinNoise::PerlinNoise()
{
    GenerateGradients();
    Seed(0);
}

PerlinNoise::PerlinNoise(unsigned int seed)
{
    GenerateGradients();
    Seed(seed);
}

void PerlinNoise::Seed(unsigned int seed)
{
    GeneratePermutations(seed);
}

static int ModuloSignInt(int a, int b)
{
    return (((a % b + b)) % b);
}

double PerlinNoise::Noise(double t, std::uint32_t frequency) const
{
    frequency = std::max(1u, frequency);

    auto Fade = [](double t) -> double
    {
        return t*t*t*(t*(t*6.0 - 15.0) + 10.0);
    };

    auto Surflet = [&](std::uint32_t i)
    {
        const auto& p = perm_;
        auto hash = p[ ModuloSignInt(i, frequency) ];

        auto dist = t - static_cast<double>(i);
        auto grad = dist * grads_[hash];
        auto poly = 1.0 - Fade(std::abs(dist));

        return poly * grad;
    };

    auto i = static_cast<std::uint32_t>(t);

    return (Surflet(i) + Surflet(i + 1));
}

double PerlinNoise::Noise(double t, std::uint32_t frequency, std::uint32_t octaves, double persistence) const
{
    frequency = std::max(1u, frequency);

    double noise        = 0.0;
    double amplitude    = 1.0;

    for (std::uint32_t i = 0; i < octaves; ++i)
    {
        noise       += Noise(t, frequency) * amplitude;
        t           *= 2.0;
        amplitude   *= persistence;
        frequency   *= 2;
    }

    return noise;
}


/*
 * ======= Private: =======
 */

void PerlinNoise::GeneratePermutations(std::uint32_t seed)
{
    static const int n = 256;

    for (int i = 0; i < n; ++i)
        perm_[i] = i;

    std::shuffle(std::begin(perm_), std::begin(perm_) + n, std::default_random_engine(seed));

    for (int i = 0; i < n; ++i)
        perm_[n + i] = perm_[i];
}

void PerlinNoise::GenerateGradients()
{
    double angleStep = M_PI * 2.0 / static_cast<double>(grads_.size());

    for (std::size_t i = 0; i < grads_.size(); ++i)
    {
        auto a = static_cast<double>(i) * angleStep;
        grads_[i] = std::cos(a);
    }
}


} // /namespace Ac



// ================================================================================
