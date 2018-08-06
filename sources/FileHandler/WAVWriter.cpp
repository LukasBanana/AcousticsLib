/*
 * WAVWriter.cpp
 * 
 * This stream is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WAVWriter.h"
#include "WAVFileFormat.h"
#include "WAVFormatTags.h"
#include <sstream>


namespace Ac
{


static void GetRIFFWAVEFormat(RIFFWAVEFormat& format, const WaveBufferFormat& fmt)
{
    format.formatTag        = RIFFWAVEFormatTags::PCM;
    format.channels         = fmt.channels;
    format.sampleRate       = fmt.sampleRate;
    format.bytesPerSecond   = fmt.BytesPerSecond();
    format.blockAlign       = static_cast<std::uint16_t>(fmt.BytesPerFrame());
    format.bitsPerSample    = fmt.bitsPerSample;
}

template <typename T>
static void Write(std::ostream& stream, const T& buffer)
{
    stream.write(reinterpret_cast<const char*>(&buffer), sizeof(T));
}

static void WAVWriteRIFFWAVEHeader(std::ostream& stream, std::uint32_t fileSize)
{
    if (fileSize <= 16)
        throw std::runtime_error("invalid size data field for RIFF WAVE stream (size = " + std::to_string(fileSize) + ")");

    /* Write magic number 'RIFF' */
    std::uint32_t magicNumber = UINT32_FROM_STRING("RIFF");
    Write(stream, magicNumber);

    /* Write stream size */
    Write(stream, fileSize);

    /* Write stream format */
    std::uint32_t formatType = UINT32_FROM_STRING("WAVE");
    Write(stream, formatType);
}

static void WAVWriteChunk(std::ostream& stream, const char* chunkID, std::uint32_t chunkSize)
{
    std::uint32_t id = UINT32_FROM_STRING(chunkID);
    Write(stream, id);
    Write(stream, chunkSize);
}

/*
RIFF WAVE format chunk (for RIFF tags see details).
\see http://de.wikipedia.org/wiki/RIFF_WAVE
\see http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/RIFF.html
*/
static void WAVWriteChunks(std::ostream& stream, const WaveBuffer& waveBuffer)
{
    /* Get RIFF WAVE format from buffer format object */
    RIFFWAVEFormat format;
    GetRIFFWAVEFormat(format, waveBuffer.GetFormat());

    /* Write "fmt " chunk */
    std::uint32_t chunkSizeFMT = sizeof(format);
    WAVWriteChunk(stream, "fmt ", chunkSizeFMT);

    Write(stream, format);

    /* Write "data" chunk and PCM data */
    std::uint32_t chunkSizeDATA = waveBuffer.BufferSize();
    WAVWriteChunk(stream, "data", chunkSizeDATA);

    stream.write(waveBuffer.Data(), waveBuffer.BufferSize());
}

void WAVWriter::WriteWaveBuffer(std::ostream& stream, const WaveBuffer& buffer)
{
    if (!stream.good())
        throw std::runtime_error("invalid output stream for WAV file");

    /* Write RIFF WAVE header */
    std::uint32_t streamSize = 4u + 2u*sizeof(RIFFWAVEChunk) + sizeof(RIFFWAVEFormat) + buffer.BufferSize();
    WAVWriteRIFFWAVEHeader(stream, streamSize);

    /* Fill wave buffer by reading chunks "fmt " and "data" */
    WAVWriteChunks(stream, buffer);
}


} // /namespace Ac



// ================================================================================
