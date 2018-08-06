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

    header.size = SwapEndian(header.size);
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
        chunk.size = SwapEndian(chunk.size);

        if (chunk.id != UINT32_FROM_STRING(chunkId))
        {
            stream.seekg(chunk.size, std::ios_base::cur);

            if (chunk.size == 0 || stream.fail())
                throw std::runtime_error("failed to find '" + std::string(chunkId) + "' chunk in AIFF/AIFF-C stream");
        }
        else
            break;
    }
}

static void AIFFReadCommonChunk(std::istream& stream, AIFFCommonChunk& chunk)
{
    Read(stream, chunk);

    chunk.channels      = SwapEndian(chunk.channels);
    chunk.sampleFrames  = SwapEndian(chunk.sampleFrames);
    chunk.bitsPerSample = SwapEndian(chunk.bitsPerSample);

    SwapEndian(chunk.sampleRate);
}

static std::string AIFCReadPString(std::istream& stream)
{
    /* Read length and fill string */
    std::uint8_t length = 0;
    Read(stream, length);

    std::string s;
    s.resize(length);

    stream.read(&s[0], length);

    /* Ignore pad byte when the number of text bytes is even (number of text bytes + length byte must be even) */
    if (length % 2 == 0)
        stream.seekg(1, std::ios_base::cur);

    return s;
}

static void AIFCReadCommonChunk(std::istream& stream, AIFCCommonChunk& chunk)
{
    Read(stream, chunk);

    auto compressionName = AIFCReadPString(stream);

    if (chunk.compressionType != UINT32_FROM_STRING("NONE"))
    {
        throw std::runtime_error(
            "unsupported compression type '" + GetStrinFromUINT32(chunk.compressionType) +
            "' (" + compressionName + ") in AIFF/AIFF-C stream"
        );
    }
}

static void AIFFReadSoundChunk(std::istream& stream, AIFFSoundChunk& chunk)
{
    Read(stream, chunk);

    chunk.offset    = SwapEndian(chunk.offset);
    chunk.blockSize = SwapEndian(chunk.blockSize);
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

    AIFCCommonChunk commChunkEx;
    if (header.formType == UINT32_FROM_STRING("AIFC"))
        AIFCReadCommonChunk(stream, commChunkEx);

    /* Read SSND chunk */
    AIFFChunk ssndChunkHdr;
    AIFFReadChunk(stream, ssndChunkHdr, "SSND");

    AIFFSoundChunk ssndChunk;
    AIFFReadSoundChunk(stream, ssndChunk);

    /* Read sound data */
    auto sampleRate = static_cast<std::uint32_t>(ReadFloat80(commChunk.sampleRate));
    auto soundDataSize = commChunk.sampleFrames * commChunk.channels * commChunk.bitsPerSample / 8;

    const WaveBufferFormat format
    {
        sampleRate,
        static_cast<std::uint16_t>(commChunk.bitsPerSample),
        static_cast<std::uint16_t>(commChunk.channels)
    };

    buffer.SetFormat(format);
    buffer.SetSampleFrames(commChunk.sampleFrames);

    if (static_cast<std::size_t>(soundDataSize) != buffer.BufferSize())
        throw std::runtime_error("inconsistent sizes of sound data buffers in AIFF/AIFF-C stream");

    stream.read(buffer.Data(), buffer.BufferSize());

    buffer.SwapEndianness();
}


} // /namespace Ac



// ================================================================================
