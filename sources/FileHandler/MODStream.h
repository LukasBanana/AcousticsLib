/*
 * MODStream.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_MOD_STREAM_H__
#define __AC_MOD_STREAM_H__


#include <Ac/AudioStream.h>
#include <memory>


namespace Ac
{


class AC_EXPORT MODStream : public AudioStream
{

    public:

        MODStream(std::unique_ptr<std::istream>&& stream);
        ~MODStream();

        std::size_t StreamWaveBuffer(WaveBuffer& buffer) override;

        void Seek(double timePoint) override;

        double TotalTime() const override;

        std::vector<std::string> InfoComments() const override;

        WaveBufferFormat GetFormat() const override;

    private:

        std::unique_ptr<std::istream> stream_;

};


} // /namespace Ac


#endif



// ================================================================================
