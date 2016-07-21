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

        Win32Microphone();
        ~Win32Microphone();

        Win32Microphone(const Win32Microphone&) = delete;
        Win32Microphone& operator = (const Win32Microphone&) = delete;

        std::unique_ptr<WaveBuffer> ReceivedInput() override;

        void Start() override;
        void Stop() override;

        bool IsRecording() const override;

        void OnSync(DWORD bytesRecorded);

    private:

        void OpenWaveInput();
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
