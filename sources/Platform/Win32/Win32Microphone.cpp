/*
 * Win32Microphone.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Microphone.h"
#include "Win32MicrophoneCallback.h"


namespace Ac
{


static const std::size_t initialBufferSize = 16384u;

Win32Microphone::Win32Microphone() :
    buffer_( initialBufferSize, 0 )
{
    OpenWaveInput();
}

Win32Microphone::~Win32Microphone()
{
    CloseWaveInput();
}

bool Win32Microphone::ProcessInput(WaveBuffer& waveBuffer)
{
    


    return false;
}


/*
 * ======= Private: =======
 */

void Win32Microphone::OpenWaveInput()
{
    /* Open wave input device */
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
    waveFormat.nChannels       = 1;
    waveFormat.nSamplesPerSec  = 11025;
    waveFormat.nAvgBytesPerSec = 11025;
    waveFormat.nBlockAlign     = 1;
    waveFormat.wBitsPerSample  = 8;
    waveFormat.cbSize          = 0; // must be 0 due to WAVE_FORMAT_PCM tag
    
    auto result = waveInOpen(
        &waveIn_,
        WAVE_MAPPER,
        &waveFormat,
        reinterpret_cast<DWORD_PTR>(Win32MicrophoneCallback),
        reinterpret_cast<DWORD_PTR>(this),
        CALLBACK_FUNCTION
    );

    if (result != MMSYSERR_NOERROR)
        throw std::runtime_error("waveInOpen: " + MMErrorToString(result));

    /* Prepare wave input header */
    waveHdr_.lpData          = reinterpret_cast<CHAR*>(buffer_.data());
    waveHdr_.dwBufferLength  = static_cast<DWORD>(buffer_.size());
    waveHdr_.dwBytesRecorded = 0;
    waveHdr_.dwUser          = 0;
    waveHdr_.dwFlags         = 0;
    waveHdr_.dwLoops         = 1;
    waveHdr_.lpNext          = nullptr;
    waveHdr_.reserved        = 0;

    result = waveInPrepareHeader(waveIn_, &waveHdr_, sizeof(WAVEHDR));

    if (result != MMSYSERR_NOERROR)
        throw std::runtime_error("waveInPrepareHeader: " + MMErrorToString(result));
}

void Win32Microphone::CloseWaveInput()
{
    waveInUnprepareHeader(waveIn_, &waveHdr_, sizeof(WAVEHDR));
    waveInClose(waveIn_);
}


} // /namespace Ac



// ================================================================================
