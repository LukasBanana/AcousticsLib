/*
 * AIFFReader.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AIFF_READER_H__
#define __AC_AIFF_READER_H__


#include "AudioReader.h"


namespace Ac
{


class AC_EXPORT AIFFReader : public AudioReader
{

    public:

        void ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer) override;

};


} // /namespace Ac


#endif



// ================================================================================
