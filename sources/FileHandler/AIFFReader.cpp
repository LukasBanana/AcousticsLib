/*
 * AIFFReader.cpp
 * 
 * This stream is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AIFFReader.h"
#include "AIFFFileFormat.h"
#include "../Core/Endianness.h"


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

static void AIFFReadChunk(std::istream& stream, AIFFChunk& chunk, const char* chunkId)
{
    if (!chunkId)
        return;

    while (true)
    {
        if (stream.eof())
            throw std::runtime_error("missing chunk ID '" + std::string(chunkId) + "' in AIFF/AIFF-C stream");

        Read(stream, chunk);
        SwapEndian(chunk.size);

        if (chunk.id != UINT32_FROM_STRING(chunkId))
            stream.seekg(chunk.size, std::ios_base::cur);
        else
            break;
    }

    //if (chunkId != nullptr && chunk.id != UINT32_FROM_STRING(chunkId))
    //    throw std::runtime_error("expected chunk ID '" + std::string(chunkId) + "' in AIFF/AIFF-C stream, but got '" + GetStrinFromUINT32(chunk.id) + "'");
}

static void AIFFReadCommonChunk(std::istream& stream, AIFFCommonChunk& chunk)
{
    Read(stream, chunk);
    
    SwapEndian(chunk.channels);
    SwapEndian(chunk.sampleFrames);
    SwapEndian(chunk.bitsPerSample);
    SwapEndian(chunk.sampleRate);
}

/*static void AIFCReadCommonChunk(std::istream& stream, AIFCCommonChunk& chunk)
{
    Read(stream, chunk);

    SwapEndian(chunk.compressionType);
}*/

static void AIFFReadSoundChunk(std::istream& stream, AIFFSoundChunk& chunk)
{
    Read(stream, chunk);

    SwapEndian(chunk.offset);
    SwapEndian(chunk.blockSize);
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
    
    /*AIFCCommonChunk commChunkEx;
    if (header.formType == UINT32_FROM_STRING("AIFC"))
        AIFCReadCommonChunk(stream, commChunkEx);*/

    /* Read SSND chunk */
    AIFFChunk ssndChunkHdr;
    AIFFReadChunk(stream, ssndChunkHdr, "SSND");

    AIFFSoundChunk ssndChunk;
    AIFFReadSoundChunk(stream, ssndChunk);

    /* Read sound data */
    auto sampleRate = static_cast<unsigned int>(ReadFloat80(commChunk.sampleRate));
    auto soundDataSize = commChunk.sampleFrames * commChunk.channels * commChunk.bitsPerSample / 8;

    buffer.SetFormat(WaveBufferFormat(sampleRate, commChunk.bitsPerSample, commChunk.channels));
    buffer.SetSampleCount(commChunk.sampleFrames);

    if (soundDataSize != buffer.BufferSize())
        throw std::runtime_error("inconsistent sizes of sound data buffers in AIFF/AIFF-C stream");

    stream.read(buffer.Data(), buffer.BufferSize());

    buffer.SwapEndianness();
}


} // /namespace Ac



// ================================================================================
