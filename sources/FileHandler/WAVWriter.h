/*
 * WAVWriter.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_WAV_WRITER_H
#define AC_WAV_WRITER_H


#include "AudioWriter.h"


namespace Ac
{


class AC_EXPORT WAVWriter : public AudioWriter
{

    public:

        void WriteWaveBuffer(std::ostream& stream, const WaveBuffer& buffer) override;

};


} // /namespace Ac


#endif



// ================================================================================
