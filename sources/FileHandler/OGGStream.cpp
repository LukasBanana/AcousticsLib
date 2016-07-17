/*
 * OGGStream.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifdef AC_PLUGIN_OGGVORBIS

#include "OGGStream.h"


namespace Ac
{


/*
Map OggVorbis error code to a string.
see https://www.xiph.org/vorbis/doc/libvorbis/return.html
*/
static std::string OggError(int err)
{
    switch (err)
    {
        case OV_FALSE:
            return "not true, or no data available";
        case OV_HOLE:
            return "vorbisfile encoutered missing or corrupt data in the bitstream; recovery is normally automatic and this return code is for informational purposes only";
        case OV_EREAD:
            return "read error while fetching compressed data for decode";
        case OV_EFAULT:
            return "internal inconsistency in encode or decode state. Continuing is likely not possible";
        case OV_EIMPL:
            return "feature not implemented";
        case OV_EINVAL:
            return "either an invalid argument, or incompletely initialized argument passed to a call";
        case OV_ENOTVORBIS:
            return "given file/data was not recognized as Ogg Vorbis data";
        case OV_EBADHEADER:
            return "file/data is apparently an Ogg Vorbis stream, but contains a corrupted or undecipherable header";
        case OV_EVERSION:
            return "bitstream format revision of the given stream is not supported";
        case OV_EBADLINK:
            return "given link exists in the Vorbis data stream, but is not decipherable due to garbage or corruption";
        case OV_ENOSEEK:
            return "given stream is not seekable";
    }
    return "unknown error";
}

#define OGG_DATASOURCE(src) reinterpret_cast<std::istream*>(src)

static size_t OggRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
    auto file = OGG_DATASOURCE(datasource);
    if (file)
    {
        file->read(reinterpret_cast<char*>(ptr), size*nmemb);
        return static_cast<size_t>(file->gcount());
    }
    return 0;
}

static int OggSeek(void* datasource, ogg_int64_t offset, int whence)
{
    auto file = OGG_DATASOURCE(datasource);
    if (file)
    {
        switch (whence)
        {
            case SEEK_SET:
                file->seekg(offset, std::ios_base::beg);
                break;
            case SEEK_CUR:
                file->seekg(offset, std::ios_base::cur);
                break;
            case SEEK_END:
                file->seekg(offset, std::ios_base::end);
                break;
        }
        return (file->fail() ? 1 : 0);
    }
    return 1;
}

static int OggClose(void* datasource)
{
    /*auto file = OGG_DATASOURCE(datasource);
    file->close();
    return file->fail() ? 1 : 0;*/
    return 0;
}

static long OggTell(void* datasource)
{
    auto file = OGG_DATASOURCE(datasource);
    return (file ? static_cast<long>(file->tellg()) : 0);
}

#undef OGG_DATASOURCE

OGGStream::OGGStream(std::istream& stream) :
    stream_( stream )
{
    /* Initialize function callbacks */
    ov_callbacks callbacks;
    
    callbacks.read_func     = OggRead;
    callbacks.seek_func     = OggSeek;
    callbacks.close_func    = OggClose;
    callbacks.tell_func     = OggTell;

    /* Open Ogg-Vorbis stream */
    auto result = ov_open_callbacks(reinterpret_cast<void*>(&stream), &file_, nullptr, 0, callbacks);
    if (result != 0)
        throw std::runtime_error(OggError(result));

    /* Extract file information */
    /*info_ = ov_info(&file_, -1);
    if (!info_)
        throw std::runtime_error("failed to retrieve information from ogg vorbis stream");*/
    
    /* Extract file comments */
    /*auto comment = ov_comment(&file_, -1);
    if (comment)
    {
        for (int i = 0; i < comment->comments; ++i)
            comments.push_back(comment->user_comments[i]);
    }*/

    /* Extract stream total time (in seconds) */
    totalTime_ = ov_time_total(&file_, -1);
}

OGGStream::~OGGStream()
{
    ov_clear(&file_);
}

std::size_t OGGStream::StreamWaveBuffer(WaveBuffer& buffer)
{
    auto& pcmData = buffer.buffer;
    std::size_t pos = 0;
    std::size_t size = pcmData.size();

    int bitStream = 0;

    while (pos < size)
    {
        /* Read next ogg vorbis chunk */
        auto result = ov_read(
            &file_,             // Ogg vorbis stream handle
            &(pcmData[pos]),    // Byte aligned buffer
            size,               // Buffer size
            0,                  // Little endian
            2,                  // 2 byte (16 bit) samples
            1,                  // Signed samples
            &bitStream          // Current bit stream section
        );

        /* Copy array into output buffer */
        if (result > 0)
        {
            auto chunkSize = static_cast<std::size_t>(result);
            pos += chunkSize;
            size -= chunkSize;
        }
        else
            break;
    }

    return pos;
}

void OGGStream::Seek(double timePoint)
{
    auto result = ov_time_seek(&file_, timePoint);
    if (result)
        throw std::runtime_error(OggError(result));
}

double OGGStream::TotalTime() const
{
    return totalTime_;
}


} // /namespace Ac


#endif



// ================================================================================
