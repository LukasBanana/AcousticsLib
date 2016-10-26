/*
 * ModuleInterface.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_MODULE_INTERFACE_H
#define AC_MODULE_INTERFACE_H


#include <Ac/Export.h>


extern "C"
{

//! Returns a raw pointer to the allocated audio system (allocated with "new" keyword)
AC_EXPORT void* Ac_AudioSystem_Alloc();

//! Returns the name of this audio system module.
AC_EXPORT const char* Ac_AudioSystem_Name();

}


#endif



// ================================================================================
