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


std::unique_ptr<Module> Module::Load(std::string moduleName)
{
    #ifdef AC_DEBUG
    moduleName += "D";
    #endif
    return std::unique_ptr<Module>(new MacOSModule(moduleName));
}

MacOSModule::MacOSModule(std::string moduleName)
{
    /* Open MacOS dynamic library (DYLIB) */
    moduleName = "lib" + moduleName + ".dylib";
    handle_ = dlopen(moduleName.c_str(), RTLD_LAZY);

    /* Check if loading has failed */
    if (!handle_)
        throw std::runtime_error("failed to load dynamic library (DYLIB) \"" + moduleName + "\"");
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
