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
        \brief Returns the received audio input from this microphone.
        \return Unique pointer to the new wave buffer or null if there is currently no more data.
        \remarks The recording process must be started with the "Start" function, before anything can be recorded. Example usage:
        \code
        // Start recording
        mic->Start();

        // Process microphone input until there is no more to process
        WaveBuffer inputBuffer, outputBuffer;
        while (mic->ProcessInput(inputBuffer))
        {
            // Append input buffer to output buffer
            outputBuffer.Append(inputBuffer);

            // Stop recording when user presses a key for instance
            if (...)
                mic->Stop();
        }
        \endcode
        \see Start
        \see Stop
        */
        virtual std::unique_ptr<WaveBuffer> ReceivedInput() = 0;

        /**
        \brief Starts the recording process.
        \param[in] waveFormat Specifies the wave buffer format which is to be used for
        the receiver buffer, which can be acquired with the "ReceivedInput" function.
        \param[in] sampleCount Specifies how many samples shall be received at once
        (independently of the number of channels). The larger this value, the larger the latency.
        \remarks Before a new recording process can be started, the previous one must be stopped.
        \see Stop
        \see IsRecording
        */
        virtual void Start(const WaveBufferFormat& waveFormat, std::size_t sampleCount) = 0;

        /**
        \brief Starts the recording process.
        \remarks Same as the other "Start" function but here the duration (in seconds) specifies the latency instead of the sample count.
        \see Start(const WaveBufferFormat&, std::size_t)
        */
        virtual void Start(const WaveBufferFormat& waveFormat, double duration) = 0;

        //! Stops the recording process.
        virtual void Stop() = 0;

        /**
        \brief Returns true if the recording process is currently running.
        \remarks The recording process can be started with the "Start" function.
        \see Start
        \see Stop
        */
        virtual bool IsRecording() const = 0;

};


} // /namespace Ac


#endif



// ================================================================================
