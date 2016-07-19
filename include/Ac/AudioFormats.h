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


enum class AudioFormats
{
    /**
    \brief Waveform audio format (WAV).
    \see http://soundfile.sapp.org/doc/WaveFormat/
    */
    WAV,
};

enum class AudioStreamFormats
{
    /**
    \brief Ogg-Vorbis audio stream format (OGG).
    \see http://vorbis.com/
    */
    OGG,

    /**
    \breif Module autio stream format (MOD).
    \see http://www.fileformat.info/format/mod/corion.htm
    */
    MOD,
};


} // /namespace Ac


#endif



// ================================================================================
