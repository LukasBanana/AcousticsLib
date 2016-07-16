/*
 * Win32Module.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Module.h"


namespace Ac
{


std::unique_ptr<Module> Module::Load(std::string moduleName)
{
    #ifdef AC_DEBUG
    moduleName += "D";
    #endif
    return std::unique_ptr<Module>(new Win32Module(moduleName));
}

Win32Module::Win32Module(std::string moduleName)
{
    /* Open Windows dynamic link library (DLL) */
    moduleName += ".dll";
    handle_ = LoadLibrary(moduleName.c_str());

    /* Check if loading has failed */
    if (!handle_)
        throw std::runtime_error("failed to load dynamic link library (DLL) \"" + moduleName + "\"");
}

Win32Module::~Win32Module()
{
    FreeLibrary(handle_);
}

void* Win32Module::LoadProcedure(const std::string& procedureName)
{
    /* Get procedure address from library module and return it as raw-pointer */
    auto procAddr = GetProcAddress(handle_, procedureName.c_str());
    return reinterpret_cast<void*>(procAddr);
}


} // /namespace Ac



// ================================================================================
