/*
 * OGGStream.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_OGG_STREAM_H__
#define __AC_OGG_STREAM_H__


#include <Ac/AudioStream.h>
#include <vorbis/vorbisfile.h>


namespace Ac
{


class AC_EXPORT OGGStream : public AudioStream
{

    public:

        OGGStream(std::istream& stream);
        ~OGGStream();

        std::size_t StreamWaveBuffer(WaveBuffer& buffer) override;

        void Seek(double timePoint) override;

    private:

        std::istream&   stream_;

        vorbis_info*    info_ = nullptr;
        OggVorbis_File  file_;

};


} // /namespace Ac


#endif



// ================================================================================
