/*
 * Sound.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/Sound.h>


namespace Ac
{


Sound::~Sound()
{
    // dummy
}

void Sound::AttachAndStoreBuffer(const std::shared_ptr<WaveBuffer>& waveBuffer)
{
    if (waveBuffer)
    {
        waveBuffer_ = waveBuffer;
        AttachBuffer(*waveBuffer);
    }
}

void Sound::DropStoredBuffer()
{
    waveBuffer_.reset();
}


} // /namespace Ac



// ================================================================================
