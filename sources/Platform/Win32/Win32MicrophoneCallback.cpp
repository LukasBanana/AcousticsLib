/*
 * Win32MicrophoneCallback.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32MicrophoneCallback.h"
#include "Win32Microphone.h"

#include <iostream>//!!!


namespace Ac
{


std::string MMErrorToString(MMRESULT err)
{
    switch (err)
    {
        case MMSYSERR_NOERROR:
            return "no error";
        case MMSYSERR_ERROR:
            return "unspecified error";
        case MMSYSERR_BADDEVICEID:
            return "specified device identifier is out of range";
        case MMSYSERR_NOTENABLED:
            return "driver not enabled";
        case MMSYSERR_ALLOCATED:
            return "specified resource is already allocated";
        case MMSYSERR_INVALHANDLE:
            return "specified device handle is invalid";
        case MMSYSERR_NODRIVER:
            return "no device driver is present";
        case MMSYSERR_NOMEM:
            return "unable to allocate or lock memory";

        case WAVERR_BADFORMAT:
            return "attempted to open with an unsupported waveform-audio format";

#if 0
MMSYSERR_NOTSUPPORTED (MMSYSERR_BASE + 8)  /* function isn't supported */
MMSYSERR_BADERRNUM    (MMSYSERR_BASE + 9)  /* error value out of range */
MMSYSERR_INVALFLAG    (MMSYSERR_BASE + 10) /* invalid flag passed */
MMSYSERR_INVALPARAM   (MMSYSERR_BASE + 11) /* invalid parameter passed */
MMSYSERR_HANDLEBUSY   (MMSYSERR_BASE + 12) /* handle being used simultaneously on another thread (eg callback) */
MMSYSERR_INVALIDALIAS (MMSYSERR_BASE + 13) /* specified alias not found */
MMSYSERR_BADDB        (MMSYSERR_BASE + 14) /* bad registry database */
MMSYSERR_KEYNOTFOUND  (MMSYSERR_BASE + 15) /* registry key not found */
MMSYSERR_READERROR    (MMSYSERR_BASE + 16) /* registry read error */
MMSYSERR_WRITEERROR   (MMSYSERR_BASE + 17) /* registry write error */
MMSYSERR_DELETEERROR  (MMSYSERR_BASE + 18) /* registry delete error */
MMSYSERR_VALNOTFOUND  (MMSYSERR_BASE + 19) /* registry value not found */
MMSYSERR_NODRIVERCB   (MMSYSERR_BASE + 20) /* driver does not call DriverCallback */
MMSYSERR_MOREDATA     (MMSYSERR_BASE + 21) /* more data to be returned */
MMSYSERR_LASTERROR    (MMSYSERR_BASE + 21) /* last error in range */
#endif
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
        /*case WIM_OPEN:
        {
        }
        break;*/

        case WIM_DATA:
        {
            /* Synchronize input wave buffer with microphone interface */
            auto waveHdr    = ((PWAVEHDR)wParam);
            auto bytes      = waveHdr->dwBytesRecorded;

            mic->OnSync(bytes);

            /* Send out new buffer */
            waveInAddBuffer(waveIn, waveHdr, sizeof(WAVEHDR));
        }
        break;

        /*case WIM_CLOSE:
        {
        }
        break;*/
    }
}


} // /namespace Ac



// ================================================================================
