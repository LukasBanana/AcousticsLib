/*
 * MODStream.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MODStream.h"
#include "MODFileFormat.h"
#include "FormatAuxiliary.h"
#include "../Core/Endianness.h"
#include <algorithm>

#include <iostream>//FOR DEBUGGING!!!


namespace Ac
{


MODStream::MODStream(std::unique_ptr<std::istream>&& stream) :
    stream_( std::move(stream) )
{
    if (!stream_->good())
        throw std::runtime_error("failed to start reading from MOD stream");

    /* Read header */
    MODHeader header;
    Read(*stream, header);

    /* Swap word to Motorola byte order (big endian) */
    for (unsigned i = 0; i < 31; ++i)
    {
        auto& rec = header.records[i];
        SwapEndian(rec.length);
        SwapEndian(rec.loopStart);
        SwapEndian(rec.loopLength);
        rec.length      *= 2;
        rec.loopStart   *= 2;
        rec.loopLength  *= 2;
    }

    /* Get number of patterns by the highest number stored in the pattern list */
    std::uint8_t numPatterns = 0;
    for (unsigned i = 0; i < 128; ++i)
        numPatterns = std::max(numPatterns, header.patternSequences[i]);

    /* Read patterns */
    for (std::uint8_t i = 0; i < numPatterns; ++i)
    {
        /* Read 64 rows for each pattern */
        for (int row = 0; row < 64; ++row)
        {
            //todo...

        }
    }


    #if 1

    for (int i = 0; i < 128; ++i)
        std::cout << (int)header.patternSequences[i] << ' ';
    std::cout << std::endl;

    std::cout << "song title: \"" << std::string(header.title) << '\"' << std::endl;

    #endif

}

MODStream::~MODStream()
{
}

std::size_t MODStream::StreamWaveBuffer(WaveBuffer& buffer)
{
    /* Setup buffer format */
    buffer.SetFormat(GetFormat());

    /* Read next data chunk */
    std::size_t bytes = 0;
    //std::size_t size = buffer.BufferSize();

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

WaveBufferFormat MODStream::GetFormat() const
{
    //todo...
    return WaveBufferFormat(10000, 16, 1);
}


} // /namespace Ac



// ================================================================================
