/*
 * AudioReader.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AUDIO_READER_H__
#define __AC_AUDIO_READER_H__


#include <Ac/WaveBuffer.h>
#include <Ac/Export.h>
#include <istream>


namespace Ac
{


//! Audio reader interface.
class AC_EXPORT AudioReader
{

    public:

        virtual ~AudioReader()
        {
        }

        /**
        \brief Reads the audio data from the specified stream and stores it in the wave buffer.
        \param[in,out] stream Specifies the input stream to read from.
        \param[out] buffer Specifies the output wave buffer.
        \throws std::runtime_exception If something went wrong while reading.
        */
        virtual void ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer) = 0;

};


} // /namespace Ac


#endif



// ================================================================================
