/*
 * WaveBufferFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_WAVE_BUFFER_FORMAT_H
#define AC_WAVE_BUFFER_FORMAT_H


#include "Export.h"

#include <cstdlib>
#include <cstdint>


namespace Ac
{


//! 96 kHz sample rate.
static const std::uint32_t sampleRate96kHz  = 96000;

//! 48 kHz sample rate (used for DVDs).
static const std::uint32_t sampleRate48kHz  = 48000;

//! 44.1 kHz sample rate.
static const std::uint32_t sampleRate44kHz  = 44100;

//! 32 kHz sample rate.
static const std::uint32_t sampleRate32kHz  = 32000;

//! 22.05 kHz sample rate.
static const std::uint32_t sampleRate22kHz  = 22050;

//! 16 kHz sample rate.
static const std::uint32_t sampleRate16kHz  = 16000;

//! 11.025 kHz sample rate.
static const std::uint32_t sampleRate11kHz  = 11025;

//! 8 kHz sample rate.
static const std::uint32_t sampleRate8kHz   = 8000;


//! Wave buffer format descriptor structure.
struct AC_EXPORT WaveBufferFormat
{
    WaveBufferFormat() = default;
    WaveBufferFormat(const WaveBufferFormat&) = default;
    WaveBufferFormat& operator = (const WaveBufferFormat&) = default;

    WaveBufferFormat(std::uint32_t sampleRate, std::uint16_t bitsPerSample, std::uint16_t channels);

    //! Returns the size (in bytes) for each sample frame (or rather sample block alignment) which is computed as follows: (channels * bitsPerSample) / 8.
    std::size_t BytesPerFrame() const;

    //! Returns the number of bytes this buffer format requires for each second: sampleRate * BytesPerFrame().
    std::size_t BytesPerSecond() const;

    //! Returns the total time (in seconds) a PCM buffer with the specified size (in bytes) requires to play with this wave buffer format.
    double TotalTime(std::size_t bufferSize) const;

    //! Returns true if this is a signed format. This is true if (bitsPerSample > 8) holds true.
    bool IsSigned() const;

    /**
    \brief Number of samples per second (in Hz). Default value is 44100.
    \remarks The commonly used sample rates are: 8 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz.
    */
    std::uint32_t sampleRate    = 44100;

    //! Number of bits per sample. Typical values are 8, 12, 16, 24, and 32. Default value is 16.
    std::uint16_t bitsPerSample = 16;

    //! Number of channels. 1 for mono and 2 for stereo. Default value is 1.
    std::uint16_t channels      = 1;
};


//! Returns true if the two wave buffer formats are equal.
AC_EXPORT bool operator == (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs);
//! Returns true if the two wave buffer formats are unequal.
AC_EXPORT bool operator != (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs);


} // /namespace Ac


#endif



// ================================================================================
