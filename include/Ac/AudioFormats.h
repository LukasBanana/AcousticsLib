/*
 * AudioFormats.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AUDIO_FORMATS_H__
#define __AC_AUDIO_FORMATS_H__


namespace Ac
{


//! Audio formats enumeration.
enum class AudioFormats
{
    /**
    \brief Waveform Audio File Format (.wav, .wave).
    \see http://soundfile.sapp.org/doc/WaveFormat/
    */
    WAVE,
    
    /**
    \brief Audio Interface File Format (.aiff, .aif, .aifc).
    \see http://www-mmsp.ece.mcgill.ca/documents/audioformats/aiff/aiff.html
    */
    AIFF,
};

//! Audio stream formats enumeration.
enum class AudioStreamFormats
{
    /**
    \brief Ogg-Vorbis audio stream format (.ogg).
    \see http://vorbis.com/
    */
    OggVorbis,

    /**
    \breif Amiga module autio stream format (.mod).
    \see http://www.fileformat.info/format/mod/corion.htm
    */
    AmigaModule,
};


} // /namespace Ac


#endif



// ================================================================================
