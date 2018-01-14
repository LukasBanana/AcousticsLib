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


struct BufferFormatFlags
{
    DWORD           flag;
    std::uint32_t   sampleRate;
    std::uint16_t   bitsPerSample;
    std::uint16_t   channels;
};

std::vector<MicrophoneDevice> Win32Microphone::QueryDevices() const
{
    std::vector<MicrophoneDevice> devices;

    /* Iterate over all wave-in device s */
    auto numDevs = waveInGetNumDevs();

    for (decltype(numDevs) i = 0; i < numDevs; ++i)
    {
        /* Query wave-in device capabilities */
        WAVEINCAPS caps;
        auto result = waveInGetDevCaps(i, &caps, sizeof(WAVEINCAPS));

        if (result != MMSYSERR_NOERROR)
            continue;

        /* Setup device descriptor */
        MicrophoneDevice dev;

        dev.name = std::string(caps.szPname);

        std::vector<BufferFormatFlags> settings
        {
            { WAVE_FORMAT_1M08 , sampleRate11kHz,  8, 1 },
            { WAVE_FORMAT_1M16 , sampleRate11kHz, 16, 1 },
            { WAVE_FORMAT_1S08 , sampleRate11kHz,  8, 2 },
            { WAVE_FORMAT_1S16 , sampleRate11kHz, 16, 2 },
            { WAVE_FORMAT_2M08 , sampleRate22kHz,  8, 1 },
            { WAVE_FORMAT_2M16 , sampleRate22kHz, 16, 1 },
            { WAVE_FORMAT_2S08 , sampleRate22kHz,  8, 2 },
            { WAVE_FORMAT_2S16 , sampleRate22kHz, 16, 2 },
            { WAVE_FORMAT_4M08 , sampleRate44kHz,  8, 1 },
            { WAVE_FORMAT_4M16 , sampleRate44kHz, 16, 1 },
            { WAVE_FORMAT_4S08 , sampleRate44kHz,  8, 2 },
            { WAVE_FORMAT_4S16 , sampleRate44kHz, 16, 2 },
            { WAVE_FORMAT_96M08, sampleRate96kHz,  8, 1 },
            { WAVE_FORMAT_96M16, sampleRate96kHz, 16, 1 },
            { WAVE_FORMAT_96S08, sampleRate96kHz,  8, 2 },
            { WAVE_FORMAT_96S16, sampleRate96kHz, 16, 2 },
        };

        for (const auto& stng : settings)
        {
            if ((caps.dwFormats & stng.flag) != 0)
                dev.formats.push_back(WaveBufferFormat { stng.sampleRate, stng.bitsPerSample, stng.channels });
        }

        devices.push_back(dev);
    }

    return devices;
}

std::unique_ptr<WaveBuffer> Win32Microphone::ReceivedInput()
{
    if (recording_)
    {
        /* The receiver buffer will be filled from another thread, so we need a lock guard here */
        std::lock_guard<std::mutex> lock { recvBufferMutex_ };
        return std::move(recvBuffer_);
    }
    return nullptr;
}

void Win32Microphone::Start(const WaveBufferFormat& waveFormat, std::size_t sampleFrames, std::size_t deviceIndex)
{
    if (!recording_)
    {
        /* Store recording wave format */
        recvBufferFormat_ = waveFormat;

        /* Start recording process */
        OpenWaveInput(sampleFrames, GetDeviceID(deviceIndex));

        waveInAddBuffer(waveIn_, &waveHdr_, sizeof(WAVEHDR));
        waveInStart(waveIn_);

        recording_ = true;
    }
}

void Win32Microphone::Start(const WaveBufferFormat& waveFormat, double duration, std::size_t deviceIndex)
{
    Start(
        waveFormat,
        static_cast<std::size_t>(duration * static_cast<double>(waveFormat.sampleRate)),
        deviceIndex
    );
}

void Win32Microphone::Stop()
{
    if (recording_)
    {
        /* Stop recording process */
        waveInStop(waveIn_);
        CloseWaveInput();
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
    std::lock_guard<std::mutex> lock { recvBufferMutex_ };

    /* Create new receiver buffer (if previous buffer was moved to user) */
    if (!recvBuffer_)
        recvBuffer_ = std::unique_ptr<WaveBuffer>(new WaveBuffer { recvBufferFormat_ });

    /* Copy input buffer to receiver buffer */
    recvBuffer_->SetSampleFrames(buffer_.size() / recvBufferFormat_.BytesPerFrame());
    std::copy(buffer_.begin(), buffer_.end(), recvBuffer_->Data());
}


/*
 * ======= Private: =======
 */

UINT Win32Microphone::GetDeviceID(std::size_t deviceIndex) const
{
    if (deviceIndex != standardDeviceIndex)
    {
        auto idx = static_cast<UINT>(deviceIndex);
        if (idx < waveInGetNumDevs())
            return idx;
    }
    return WAVE_MAPPER;
}

void Win32Microphone::OpenWaveInput(std::size_t sampleFrames, UINT deviceID)
{
    /* Resize output buffer first (to avoid race condition with thread of the recording callback) */
    buffer_.resize(sampleFrames * recvBufferFormat_.BytesPerFrame());

    /* Open wave input device */
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
    waveFormat.nChannels       = recvBufferFormat_.channels;
    waveFormat.nSamplesPerSec  = recvBufferFormat_.sampleRate;
    waveFormat.nAvgBytesPerSec = recvBufferFormat_.BytesPerSecond();
    waveFormat.nBlockAlign     = static_cast<WORD>(recvBufferFormat_.BytesPerFrame());
    waveFormat.wBitsPerSample  = recvBufferFormat_.bitsPerSample;
    waveFormat.cbSize          = 0; // must be 0 due to WAVE_FORMAT_PCM tag
    
    auto result = waveInOpen(
        &waveIn_,
        deviceID,
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
