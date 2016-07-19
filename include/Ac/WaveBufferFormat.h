/*
 * WaveBufferFormat.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_BUFFER_FORMAT_H__
#define __AC_WAVE_BUFFER_FORMAT_H__


#include "Export.h"

#include <cstdlib>


namespace Ac
{


//! Wave buffer format descriptor structure.
struct AC_EXPORT WaveBufferFormat
{
    WaveBufferFormat() = default;
    WaveBufferFormat(const WaveBufferFormat&) = default;
    WaveBufferFormat& operator = (const WaveBufferFormat&) = default;

    WaveBufferFormat(unsigned int sampleRate, unsigned short bitsPerSample, unsigned short channels);

    //! Returns the sample block alignment: (channels * bitsPerSample) / 8.
    std::size_t BlockAlign() const;

    //! Returns the number of bytes this buffer format requires for each second: sampleRate * BlockAlign().
    std::size_t BytesPerSecond() const;

    //! Returns the total time (in seconds) a PCM buffer with the specified size (in bytes) requires to play with this wave buffer format.
    double TotalTime(std::size_t bufferSize) const;

    //! Returns true if this is a signed format. This is true if (bitsPerSample > 8) holds true.
    bool IsSigned() const;

    /**
    \brief Number of samples per second (in Hz). Default value is 44100.
    \remarks The commonly used sample rates are: 8 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz.
    */
    unsigned int    sampleRate      = 44100;

    //! Number of bits per sample. Typical values are 8, 12, 16, 24, and 32. Default value is 16.
    unsigned short  bitsPerSample   = 16;

    //! Number of channels. 1 for mono and 2 for stereo. Default value is 1.
    unsigned short  channels        = 1;
};


//! Returns true if the two wave buffer formats are equal.
AC_EXPORT bool operator == (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs);
//! Returns true if the two wave buffer formats are unequal.
AC_EXPORT bool operator != (const WaveBufferFormat& lhs, const WaveBufferFormat& rhs);


} // /namespace Ac


#endif



// ================================================================================
