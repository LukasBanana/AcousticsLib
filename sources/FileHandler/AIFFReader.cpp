/*
 * AIFFReader.cpp
 * 
 * This stream is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AIFFReader.h"
#include "AIFFFileFormat.h"


namespace Ac
{

    
static void AIFFReadHeader(std::istream& stream, AIFFHeader& header)
{
    Read(stream, header);
    
    if (header.id != UINT32_FROM_STRING("FORM"))
        throw std::runtime_error("invalid magic number in AIFF/AIFF-C stream");
    
    if (header.formType != UINT32_FROM_STRING("AIFF") && header.formType != UINT32_FROM_STRING("AIFC"))
        throw std::runtime_error("invalid form type in AIFF/AIFF-C stream (must be either 'AIFF' or 'AIFC')");
    
    SwapEndian(header.size);
}

static void AIFFReadChunk(std::istream& stream, AIFFChunk& chunk, const char* chunkId = nullptr)
{
    Read(stream, chunk);
    
    if (chunkId != nullptr && chunk.id != UINT32_FROM_STRING(chunkId))
        throw std::runtime_error("expected chunk ID '" + std::string(chunkId) + "', but got '" + GetStrinFromUINT32(chunk.id) + "'");
    
    SwapEndian(chunk.size);
}

static void AIFFReadCommonChunk(std::istream& stream, AIFFCommonChunk& chunk)
{
    Read(stream, chunk);
    
    SwapEndian(chunk.channels);
    SwapEndian(chunk.sampleFrames);
    SwapEndian(chunk.bitsPerSample);
    SwapEndian(chunk.sampleRate);
    
    auto rate = ReadFloat80(chunk.sampleRate);
}

void AIFFReader::ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer)
{
    if (!stream.good())
        throw std::runtime_error("invalid input stream for AIFF file");

    /* Read AIFF header */
    AIFFHeader header;
    AIFFReadHeader(stream, header);
    
    /* Read COMM chunk */
    AIFFChunk commChunkHdr;
    AIFFReadChunk(stream, commChunkHdr, "COMM");
    
    AIFFCommonChunk commChunk;
    AIFFReadCommonChunk(stream, commChunk);
    
    
    /* Read SSND chunk */
    
    
}


} // /namespace Ac



// ================================================================================
