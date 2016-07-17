/*
 * MacOSModule.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MacOSModule.h"
#include <dlfcn.h>


namespace Ac
{


std::string Module::GetModuleFilename(std::string moduleName)
{
    /* Extend module name to MacOS dynamic library name (DYLIB) */
    #ifdef AC_DEBUG
    moduleName += "D";
    #endif
    return "libAcLib_" + moduleName + ".dylib";
}

bool Module::IsAvailable(const std::string& moduleFilename)
{
    /* Check if MacOS dynamic library can be loaded properly */
    auto handle = dlopen(moduleFilename.c_str(), RTLD_LAZY);
    if (handle)
    {
        dlclose(handle);
        return true;
    }
    return false;
}

std::unique_ptr<Module> Module::Load(const std::string& moduleFilename)
{
    return std::unique_ptr<Module>(new MacOSModule(moduleFilename));
}

MacOSModule::MacOSModule(const std::string& moduleFilename)
{
    /* Open MacOS dynamic library */
    handle_ = dlopen(moduleFilename.c_str(), RTLD_LAZY);

    /* Check if loading has failed */
    if (!handle_)
        throw std::runtime_error("failed to load dynamic library (DYLIB) \"" + moduleFilename + "\"");
}

MacOSModule::MacOSModule()
{
    dlclose(handle_);
}

void* MacOSModule::LoadProcedure(const std::string& procedureName)
{
    /* Get procedure address from library module and return it as raw-pointer */
    auto procAddr = dlsym(handle_, procedureName.c_str());
    return reinterpret_cast<void*>(procAddr);
}


} // /namespace Ac



// ================================================================================
