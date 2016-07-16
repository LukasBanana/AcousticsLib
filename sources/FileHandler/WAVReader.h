/*
 * WAVReader.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAV_READER_H__
#define __AC_WAV_READER_H__


#include "../Core/WaveBuffer.h"

#include <Ac/Export.h>
#include <string>
#include <istream>


namespace Ac
{


class AC_EXPORT WAVReader
{

    public:

        void ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer);

};


} // /namespace Ac


#endif



// ================================================================================
