/*
 * NullModuleInterface.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../ModuleInterface.h"
#include "NullAudioSystem.h"


extern "C"
{

AC_EXPORT void* Ac_AudioSystem_Alloc()
{
    return new Ac::NullAudioSystem();
}

AC_EXPORT const char* Ac_AudioSystem_Name()
{
    return "Null";
}

}



// ================================================================================
