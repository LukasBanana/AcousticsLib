/*
 * MODStream.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MODStream.h"
#include "MODFileFormat.h"
#include <algorithm>

#include <iostream>//FOR DEBUGGING!!!


namespace Ac
{


MODStream::MODStream(std::istream& stream) :
    stream_( stream )
{
    if (!stream_.good())
        throw std::runtime_error("failed to start reading from MOD stream");

    /* Read header */
    MODHeader header;
    stream_.read(reinterpret_cast<char*>(&header), sizeof(header));

    /* Get number of patterns by the highest number stored in the pattern list */
    std::uint8_t numPatterns = 0;
    for (int i = 0; i < 128; ++i)
        numPatterns = std::max(numPatterns, header.patternSequences[i]);

    /* Read patterns */
    for (std::uint8_t i = 0; i < numPatterns; ++i)
    {
        /* Read 64 rows for each pattern */
        for ()
        {


        }
    }


    for (int i = 0; i < 128; ++i)
        std::cout << (int)header.patternSequences[i] << ", ";
    std::cout << std::endl;

    std::cout << "song title: \"" << std::string(header.title) << '\"' << std::endl;



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
