/*
 * AudioStream.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AUDIO_STREAM_H__
#define __AC_AUDIO_STREAM_H__


#include <Ac/WaveBuffer.h>
#include <Ac/Export.h>
#include <istream>
#include <vector>
#include <string>


namespace Ac
{


//! Audio stream interface.
class AC_EXPORT AudioStream
{

    public:

        virtual ~AudioStream()
        {
        }

        /**
        \brief Reads the audio data from the active stream and stores it in the wave buffer.
        \param[out] buffer Specifies the output wave buffer.
        \return Number of bytes read from the input stream. If this is zero, the end of the stream has been reached.
        \throws std::runtime_exception If something went wrong while reading.
        */
        virtual std::size_t StreamWaveBuffer(WaveBuffer& buffer) = 0;

        /**
        \briefs Sets the new time point from where to stream the audio data.
        \param[in] timePoint Specifies the new position (in seconds).
        \throws std::runtime_exception If something went wrong.
        */
        virtual void Seek(double timePoint) = 0;

        //! Returns the total time of the stream (in seconds).
        virtual double TotalTime() const = 0;

        //! Returns an optional list of strings, containing informational commentaries such as "ARTIST=John Doe".
        virtual std::vector<std::string> InfoComments() const = 0;

        //! Returns the native wave buffer format of this audio stream.
        virtual WaveBufferFormat GetFormat() const = 0;

};


} // /namespace Ac


#endif



// ================================================================================
