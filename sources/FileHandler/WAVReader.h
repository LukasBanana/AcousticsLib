/*
 * WAVReader.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAV_READER_H__
#define __AC_WAV_READER_H__


#include <Ac/AudioReader.h>


namespace Ac
{


class AC_EXPORT WAVReader : public AudioReader
{

    public:

        void ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer) override;

};


} // /namespace Ac


#endif



// ================================================================================
