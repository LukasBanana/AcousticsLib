/*
 * MODStream.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MODStream.h"


namespace Ac
{


MODStream::MODStream(std::istream& stream) :
    stream_( stream )
{

}

MODStream::~MODStream()
{
}

std::size_t MODStream::StreamWaveBuffer(WaveBuffer& buffer)
{
    /* Setup buffer format */
    WaveBufferFormat format;

    format.channels         = 1;
    format.sampleRate       = 10000;
    format.bitsPerSample    = 16;

    buffer.SetFormat(format);

    /* Read next data chunk */
    std::size_t bytes = 0;
    std::size_t size = buffer.BufferSize();

    //todo...

    return bytes;
}

void MODStream::Seek(double timePoint)
{
    //todo...
}

double MODStream::TotalTime() const
{
    return 0.0;
}

std::vector<std::string> MODStream::InfoComments() const
{
    return {};
}


} // /namespace Ac



// ================================================================================
