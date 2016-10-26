/*
 * AudioWriter.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AUDIO_WRITER_H
#define AC_AUDIO_WRITER_H


#include <Ac/WaveBuffer.h>
#include <Ac/Export.h>
#include <ostream>


namespace Ac
{


//! Audio writer interface.
class AC_EXPORT AudioWriter
{

    public:

        virtual ~AudioWriter()
        {
        }

        /**
        \brief Writes the audio data to the specified stream.
        \param[in,out] stream Specifies the output stream to write to.
        \param[in] buffer Specifies the input wave buffer.
        \throws std::runtime_exception If something went wrong while writing.
        */
        virtual void WriteWaveBuffer(std::ostream& stream, const WaveBuffer& waveBuffer) = 0;

};


} // /namespace Ac


#endif



// ================================================================================
