/*
 * Module.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_MODULE_H
#define AC_MODULE_H


#include <memory>
#include <string>


namespace Ac
{


#ifdef _WIN32
#   define AC_PROC_INTERFACE(RET, NAME, ARG_LIST) typedef RET (__cdecl *NAME) ARG_LIST
#else
#   define AC_PROC_INTERFACE(RET, NAME, ARG_LIST) typedef RET (*NAME) ARG_LIST
#endif


//! Module class (to load procedures for shared libraries)
class Module
{

    public:

        Module() = default;

        Module(const Module&) = delete;
        Module& operator = (const Module&) = delete;

        virtual ~Module()
        {
        }

        //! Converts the module name into a specific filename (e.g. "OpenAL" to "AcLib_OpenAL.dll" on Windows).
        static std::string GetModuleFilename(std::string moduleName);

        //! Returns true if the specified module is available.
        static bool IsAvailable(const std::string& moduleFilename);

        //! Returns the specified module or null if it is not available.
        static std::unique_ptr<Module> Load(const std::string& moduleFilename);

        //! Returns a raw pointer to the specified procedure loaded from this module.
        virtual void* LoadProcedure(const std::string& procedureName) = 0;

};


} // /namespace Ac


#endif



// ================================================================================
