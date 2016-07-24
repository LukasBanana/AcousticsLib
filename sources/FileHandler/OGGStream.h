/*
 * OGGStream.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_OGG_STREAM_H__
#define __AC_OGG_STREAM_H__

#ifdef AC_PLUGIN_OGGVORBIS


#include <Ac/AudioStream.h>
#include <memory>
#include <vorbis/vorbisfile.h>


namespace Ac
{


class AC_EXPORT OGGStream : public AudioStream
{

    public:

        OGGStream(std::unique_ptr<std::istream>&& stream);
        ~OGGStream();

        std::size_t StreamWaveBuffer(WaveBuffer& buffer) override;

        void Seek(double timePoint) override;

        double TotalTime() const override;

        std::vector<std::string> InfoComments() const override;

        WaveBufferFormat GetFormat() const override;

    private:

        std::unique_ptr<std::istream>   stream_;

        vorbis_info*                    info_       = nullptr;
        OggVorbis_File                  file_;

        double                          totalTime_  = 0.0;
        std::vector<std::string>        comments_;

};


} // /namespace Ac


#endif

#endif



// ================================================================================
