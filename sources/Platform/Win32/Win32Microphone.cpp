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

std::unique_ptr<WaveBuffer> Win32Microphone::ReceivedInput()
{
    if (recording_)
    {
        /* The receiver buffer will be filled from another thread, so we need a lock guard here */
        std::lock_guard<std::mutex> lock(recvBufferMutex_);
        return std::move(recvBuffer_);
    }
    return nullptr;
}

void Win32Microphone::Start()
{
    if (!recording_)
    {
        waveInAddBuffer(waveIn_, &waveHdr_, sizeof(WAVEHDR));
        waveInStart(waveIn_);
        recording_ = true;
    }
}

void Win32Microphone::Stop()
{
    if (recording_)
    {
        waveInStop(waveIn_);
        recording_ = false;
    }
}

bool Win32Microphone::IsRecording() const
{
    return recording_;
}

void Win32Microphone::OnSync(DWORD bytesRecorded)
{
    /* This function is called from another thread, so we need to have a lock guard for the receiver buffer */
    std::lock_guard<std::mutex> lock(recvBufferMutex_);

    /* Create new receiver buffer (if previous buffer was moved to user) */
    if (!recvBuffer_)
        recvBuffer_ = std::unique_ptr<WaveBuffer>(new WaveBuffer(recvBufferFormat_));

    /* Copy input buffer to receiver buffer */
    recvBuffer_->SetSampleCount(buffer_.size() / recvBufferFormat_.BlockAlign());
    std::copy(buffer_.begin(), buffer_.end(), recvBuffer_->Data());
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
    waveFormat.nAvgBytesPerSec = 11025*2;
    waveFormat.nBlockAlign     = 2; // 1 for 8-bit
    waveFormat.wBitsPerSample  = 16; // 8 for 8-bit
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

    /* Setup receiver buffer format */
    recvBufferFormat_.sampleRate    = 11025;
    recvBufferFormat_.bitsPerSample = 16;
    recvBufferFormat_.channels      = 1;
}

void Win32Microphone::CloseWaveInput()
{
    waveInUnprepareHeader(waveIn_, &waveHdr_, sizeof(WAVEHDR));
    waveInClose(waveIn_);
}


} // /namespace Ac



// ================================================================================
