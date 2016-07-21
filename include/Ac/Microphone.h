/*
 * Microphone.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_MICROPHONE_H__
#define __AC_MICROPHONE_H__


#include "Export.h"
#include "WaveBuffer.h"


namespace Ac
{


//! Microphone interface.
class AC_EXPORT Microphone
{

    public:

        Microphone() = default;

        Microphone(const Microphone&) = delete;
        Microphone& operator = (const Microphone&) = delete;

        virtual ~Microphone()
        {
        }

        /**
        \brief Processes the microphone audio input and stores it in the specified wave buffer.
        \param[out] waveBuffer Specifies the resulting wave buffer.
        The wave buffer size and its format will be set entirely by this function.
        \return True if there was some new input to process. Otherwise the output buffer will not be modified.
        \remarks Example usage:
        \code
        // Process microphone input until there is no more to process
        WaveBuffer inputBuffer, outputBuffer;
        while (mic->ProcessInput(inputBuffer))
        {
            // Append input buffer to output buffer
            outputBuffer.Append(inputBuffer);
        }
        \endcode
        */
        virtual bool ProcessInput(WaveBuffer& waveBuffer) = 0;

};


} // /namespace Ac


#endif



// ================================================================================
