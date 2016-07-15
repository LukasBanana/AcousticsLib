/*
 * WaveBuffer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_BUFFER_H__
#define __AC_WAVE_BUFFER_H__


#include "WaveFormat.h"

#include <vector>
#include <queue>
#include <memory>


namespace Ac
{


//! Raw audio PCM (Pulse Modulation Code) buffer type.
using PCMBuffer = std::vector<char>;

//! Data model structure for an audio wave buffer.
struct WaveBuffer
{
    //! Reverses the raw buffer.
    void Reverse();
    
    //! Converts the raw buffer to mono sound (1 channel).
    void MakeMono();

    //! Converts the raw buffer to stereo sound (2 channels).
    void MakeStereo();

    //! Returns the number of samples (independently of the number of channels).
    std::size_t NumSamples() const;

    //! Returns the total time (in seconds) which is required to play the entire wave buffer.
    double TotalTime() const;

    WaveFormat  format; //!< Wave buffer format.
    PCMBuffer   buffer; //!< Wave PCM (Pulse Modulation Code) buffer.
};


} // /namespace Ac


#endif



// ================================================================================
