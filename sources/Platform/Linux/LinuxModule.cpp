/*
 * LinuxModule.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "LinuxModule.h"
#include <dlfcn.h>


namespace Ac
{


std::unique_ptr<Module> Module::Load(std::string moduleName)
{
    #ifdef _DEBUG
    moduleName += "D";
    #endif
    return std::unique_ptr<Module>(new LinuxModule(moduleName));
}

LinuxModule::LinuxModule(std::string moduleName)
{
    /* Open Linux shared library (SO) */
    moduleName = "lib" + moduleName + ".so";
    handle_ = dlopen(moduleName.c_str(), RTLD_LAZY);

    /* Check if loading has failed */
    if (!handle_)
        throw std::runtime_error("failed to load shared library (SO) \"" + moduleName + "\"");
}

LinuxModule::~LinuxModule()
{
    dlclose(handle_);
}

void* LinuxModule::LoadProcedure(const std::string& procedureName)
{
    /* Get procedure address from library module and return it as raw-pointer */
    auto procAddr = dlsym(handle_, procedureName.c_str());
    return reinterpret_cast<void*>(procAddr);
}


} // /namespace Ac



// ================================================================================
