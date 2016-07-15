/*
 * WaveFormat.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WaveFormat.h"
#include "WaveFormatTags.h"


namespace Ac
{


WaveFormat MakeWaveFormatHeaderPCM(
    unsigned short channels, unsigned long sampleRate, unsigned short bitsPerSample)
{
    WaveFormat waveFormat;

    waveFormat.formatTag        = WaveFormatTags::PCM;
    waveFormat.channels         = channels;
    waveFormat.sampleRate       = sampleRate;
    waveFormat.bitsPerSample    = bitsPerSample;
    waveFormat.blockAlign       = (waveFormat.channels * waveFormat.bitsPerSample) / 8;
    waveFormat.bytesPerSecond   = waveFormat.sampleRate * waveFormat.blockAlign;

    return waveFormat;
}


} // /namespace Ac



// ================================================================================
