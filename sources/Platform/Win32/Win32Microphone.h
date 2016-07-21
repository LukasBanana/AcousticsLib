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

        bool ProcessInput(WaveBuffer& waveBuffer) override;

        inline WAVEHDR* GetWaveHdr()
        {
            return (&waveHdr_);
        }

    private:

        void OpenWaveInput();
        void CloseWaveInput();

        HWAVEIN             waveIn_ = nullptr;
        WAVEHDR             waveHdr_;
        std::vector<char>   buffer_;

};


} // /namespace Ac


#endif



// ================================================================================
