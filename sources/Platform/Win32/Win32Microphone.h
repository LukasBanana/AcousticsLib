/*
 * Win32Microphone.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WIN32_MICROPHONE_H__
#define __AC_WIN32_MICROPHONE_H__


#include <Ac/Microphone.h>
#include <Windows.h>
#include <vector>
#include <memory>
#include <mutex>


namespace Ac
{


//! Microphone interface.
class Win32Microphone : public Microphone
{

    public:

        Win32Microphone() = default;

        Win32Microphone(const Win32Microphone&) = delete;
        Win32Microphone& operator = (const Win32Microphone&) = delete;

        std::vector<MicrophoneDevice> QueryDevices() const override;

        std::unique_ptr<WaveBuffer> ReceivedInput() override;

        void Start(const WaveBufferFormat& waveFormat, std::size_t sampleFrames, std::size_t deviceIndex) override;
        void Start(const WaveBufferFormat& waveFormat, double duration, std::size_t deviceIndex) override;

        void Stop() override;

        bool IsRecording() const override;

        void OnSync(DWORD bytesRecorded);

    private:

        UINT GetDeviceID(std::size_t deviceIndex) const;

        void OpenWaveInput(std::size_t sampleFrames, UINT deviceID);
        void CloseWaveInput();

        HWAVEIN                     waveIn_     = nullptr;
        WAVEHDR                     waveHdr_;
        std::vector<char>           buffer_;    //!< To this buffer will be written from another thread (by the WinAPI)

        bool                        recording_  = false;

        std::unique_ptr<WaveBuffer> recvBuffer_;
        WaveBufferFormat            recvBufferFormat_;
        std::mutex                  recvBufferMutex_;

};


} // /namespace Ac


#endif



// ================================================================================
