/*
 * Win32MicrophoneCallback.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32MicrophoneCallback.h"
#include "Win32Microphone.h"


namespace Ac
{


std::string MMErrorToString(MMRESULT err)
{
    switch (err)
    {
        case MMSYSERR_ALLOCATED:
            return "specified resource is already allocated";
        case MMSYSERR_BADDEVICEID:
            return "specified device identifier is out of range";
        case MMSYSERR_NODRIVER:
            return "no device driver is present";
        case MMSYSERR_NOMEM:
            return "unable to allocate or lock memory";
        case WAVERR_BADFORMAT:
            return "attempted to open with an unsupported waveform-audio format";
        case MMSYSERR_NOERROR:
            return "no error";
    }
    return "";
}
    
void CALLBACK Win32MicrophoneCallback(HWAVEIN waveIn, UINT msg, DWORD instance, DWORD wParam, DWORD lParam)
{
    /* Get microphone object from user data field 'instance' */
    auto mic = reinterpret_cast<Win32Microphone*>(instance);
    if (!mic)
        return;

    switch (msg)
    {
        case WIM_OPEN:
        {
            waveInAddBuffer(waveIn, mic->GetWaveHdr(), sizeof(WAVEHDR));
            waveInStart(waveIn);
        }
        break;

        case WIM_DATA:
        {
            //((PWAVEHDR)lParam);


        }
        break;

        case WIM_CLOSE:
        {
            waveInStop(waveIn);
        }
        break;
    }
}


} // /namespace Ac



// ================================================================================
