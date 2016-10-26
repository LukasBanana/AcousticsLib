/*
 * Win32MicrophoneCallback.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_WIN32_MICROPHONE_CALLBACK_H
#define AC_WIN32_MICROPHONE_CALLBACK_H


#include <Windows.h>
#include <mmsystem.h>
#include <string>


namespace Ac
{


std::string MMErrorToString(MMRESULT err);

void CALLBACK Win32MicrophoneCallback(HWAVEIN waveIn, UINT msg, DWORD instance, DWORD wParam, DWORD lParam);


} // /namespace Ac


#endif



// ================================================================================
