/*
 * WaveBuffer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_BUFFER_H__
#define __AC_WAVE_BUFFER_H__


#include "Export.h"
#include "WaveFormat.h"

#include <vector>
#include <queue>
#include <memory>


namespace Ac
{


//! Raw audio PCM (Pulse Modulation Code) buffer type.
using PCMBuffer = std::vector<char>;

//! Data model structure for an audio wave buffer.
struct AC_EXPORT WaveBuffer
{
    //! Reverses the raw buffer.
    void Reverse();
    
    //! Converts the raw buffer to mono sound (1 channel).
    void MakeMono();

    //! Converts the raw buffer to stereo sound (2 channels).
    void MakeStereo();

    //! Returns the number of samples (independently of the number of channels).
    std::size_t NumSamples() const;

    //! Returns the total time (in seconds) which is required to play this entire wave buffer.
    double TotalTime() const;

    //! Returns the total time (in seconds) which is required to play an entire wave buffer with the specified configuration.
    static double TotalTime(std::size_t bufferSize, std::size_t sampleRate, std::size_t channels, std::size_t bitsPerSample);

    /**
    \brief Generates a PCM wave format.
    \param[in] channels Specifies the number of channels.
    This is commonly 1 (for mono) or 2 (for stereo) sounds.
    \param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
    \param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
    \return The generated wave format. All members are initialized with
    the respective values depending on the input parameters.
    \see WaveFormat
    \see WaveFormatTags::PCM
    */
    static WaveFormat MakeWaveFormatPCM(unsigned short channels, unsigned long sampleRate, unsigned short bitsPerSample);

    WaveFormat  format; //!< Wave buffer format.
    PCMBuffer   buffer; //!< Wave PCM (Pulse Modulation Code) buffer.
};


} // /namespace Ac


#endif



// ================================================================================
