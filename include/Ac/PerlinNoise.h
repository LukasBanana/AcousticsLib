/*
 * PerlinNoise.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_PERLIN_NOISE_H
#define AC_PERLIN_NOISE_H


#include <Ac/Export.h>
#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>


namespace Ac
{


// Perlin noise generator class.
class AC_EXPORT PerlinNoise
{

    public:

        PerlinNoise();
        PerlinNoise(unsigned int seed);

        //! Sets the random seed. By default 0.
        void Seed(unsigned int seed);

        //! Returns a noise pattern in the range [-1, +1].
        double Noise(double t, std::uint32_t frequency) const;

        //! Returns a noise pattern in the range [-1, +1] with an accumulation of \c octaves samples.
        double Noise(double t, std::uint32_t frequency, std::uint32_t octaves, double persistence = 0.5) const;

    private:

        void GeneratePermutations(std::uint32_t seed);
        void GenerateGradients();

    private:

        std::array<std::uint32_t, 512>  perm_;
        std::array<double, 256>         grads_;

};


} // /namespace Ac


#endif



// ================================================================================
