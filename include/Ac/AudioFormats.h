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
    //! Unknown audio format.
    Unknown,

    /* ----- Buffers ----- */

    /**
    \brief Waveform Audio File Format (RIFF WAVE) (.wav, .wave).
    \see http://soundfile.sapp.org/doc/WaveFormat/
    */
    WAVE,
    
    /**
    \brief Audio Interface File Format (AIFF) (.aiff, .aif, .aifc).
    \see http://www-mmsp.ece.mcgill.ca/documents/audioformats/aiff/aiff.html
    */
    AIFF,

    /**
    \brief Audio Interface File Format (AIFF-C) (.aiff, .aif, .aifc).
    \see http://www-mmsp.ece.mcgill.ca/documents/audioformats/aiff/aiff.html
    */
    AIFC,

    /* ----- Streams ----- */

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
