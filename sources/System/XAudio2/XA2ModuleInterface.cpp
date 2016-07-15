/*
 * XA2ModuleInterface.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../ModuleInterface.h"
#include "XA2AudioSystem.h"


extern "C"
{

AC_EXPORT void* Ac_AudioSystem_Alloc()
{
    return new Ac::XA2AudioSystem();
}

AC_EXPORT const char* Ac_AudioSystem_Name()
{
    return "XAudio2";
}

}



// ================================================================================
