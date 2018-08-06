/*
 * WAVReader.cpp
 * 
 * This stream is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WAVReader.h"
#include "WAVFileFormat.h"
#include "WAVFormatTags.h"
#include <sstream>


namespace Ac
{


static WaveBufferFormat GetBufferFormat(const RIFFWAVEFormat& fmt)
{
    return WaveBufferFormat(fmt.sampleRate, fmt.bitsPerSample, fmt.channels);
}

static void WAVReadHeader(std::istream& stream, std::uint32_t& fileSize)
{
    /* Read magic number 'RIFF' */
    std::uint32_t magicNumber = 0;
    Read(stream, magicNumber);

    if (magicNumber != UINT32_FROM_STRING("RIFF"))
        throw std::runtime_error("invalid magic number in RIFF WAVE stream");

    /* Read stream size */
    fileSize = 0;
    Read(stream, fileSize);

    if (fileSize <= 16)
        throw std::runtime_error("invalid size data field in RIFF WAVE stream (size = " + std::to_string(fileSize) + ")");

    /* Read stream format */
    std::uint32_t formatType = 0;
    Read(stream, formatType);

    if (formatType != UINT32_FROM_STRING("WAVE"))
        throw std::runtime_error("invalid format type in RIFF WAVE stream");
}

static RIFFWAVEChunk WAVFindChunk(std::istream& stream, std::streamoff streamSize, const char* chunkID)
{
    RIFFWAVEChunk chunk;

    std::streamoff offset = 12;

    while (offset < streamSize)
    {
        /* Read next chunk header (workaround with tmpID and tmpSize necessary due to packed fields) */
        stream.seekg(offset, std::ios::beg);

        std::uint32_t tmpID = 0;
        Read(stream, tmpID);
        chunk.id = tmpID;

        std::uint32_t tmpSize = 0;
        Read(stream, tmpSize);
        chunk.size = tmpSize;

        if (chunk.id == UINT32_FROM_STRING(chunkID))
            return chunk;

        /*
        Add offset to find next chunk (guarantee WORD padding alignment)
        -> see http://www.win32developer.com/tutorial/xaudio/xaudio_tutorial_1.shtm
        */
        chunk.size += 9;
        chunk.size &= 0xfffffffe;

        offset += chunk.size;
    }

    throw std::runtime_error("missing RIFF WAVE chunk '" + std::string(chunkID) + "'");

    return chunk;
}

/*
RIFF WAVE format chunk (for RIFF tags see details).
\see http://de.wikipedia.org/wiki/RIFF_WAVE
\see http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/RIFF.html
*/
static void WAVReadChunks(std::istream& stream, std::streamoff streamSize, WaveBuffer& waveBuffer)
{
    /* Read "fmt " chunk */
    auto chunkFMT = WAVFindChunk(stream, streamSize, "fmt ");

    RIFFWAVEFormat format;
    Read(stream, format);

    /* Validate format chunk */
    if (chunkFMT.size < 16)
        throw std::runtime_error("invalid length in RIFF WAVE format chunk");

    if (format.formatTag != RIFFWAVEFormatTags::PCM)
    {
        std::stringstream s;
        s << "unsupported RIFF WAVE format tag (0x" << std::hex << format.formatTag << ")";
        throw std::runtime_error(s.str());
    }

    /* Read "data" chunk */
    auto chunkDATA = WAVFindChunk(stream, streamSize, "data");

    /* Read PCM data */
    waveBuffer.SetFormat(GetBufferFormat(format));
    waveBuffer.SetSampleFrames(chunkDATA.size / waveBuffer.GetFormat().BytesPerFrame());
    stream.read(waveBuffer.Data(), chunkDATA.size);
}

void WAVReader::ReadWaveBuffer(std::istream& stream, WaveBuffer& buffer)
{
    if (!stream.good())
        throw std::runtime_error("invalid input stream for WAV file");

    /* Read RIFF WAVE header */
    std::uint32_t streamSize = 0;
    WAVReadHeader(stream, streamSize);

    /* Fill wave buffer by reading chunks "fmt " and "data" */
    WAVReadChunks(stream, streamSize, buffer);
}


} // /namespace Ac



// ================================================================================
