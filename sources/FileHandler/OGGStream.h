/*
 * OGGStream.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_OGG_STREAM_H__
#define __AC_OGG_STREAM_H__


#include <Ac/WaveBuffer.h>
#include <Ac/Export.h>
#include <string>
#include <istream>
#include <vorbis/vorbisfile.h>


namespace Ac
{


class AC_EXPORT OGGStream
{

    public:

        OGGStream(std::istream& stream);
        ~OGGStream();

        std::size_t StreamWaveBuffer(WaveBuffer& buffer);

    private:

        std::istream& stream_;

};


} // /namespace Ac


#endif



// ================================================================================
