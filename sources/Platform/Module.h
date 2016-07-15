/*
 * Module.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_MODULE_H__
#define __AC_MODULE_H__


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

        static std::unique_ptr<Module> Load(std::string moduleName);

        virtual void* LoadProcedure(const std::string& procedureName) = 0;

};


} // /namespace Ac


#endif



// ================================================================================
