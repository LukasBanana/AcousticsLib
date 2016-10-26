/*
 * WAVReader.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_WAV_READER_H
#define AC_WAV_READER_H


#include "AudioReader.h"


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
