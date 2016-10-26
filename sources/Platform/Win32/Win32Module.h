/*
 * Win32Module.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_WIN32_MODULE_H
#define AC_WIN32_MODULE_H


#include "../Module.h"

#include <Windows.h>


namespace Ac
{


class Win32Module : public Module
{

    public:

        Win32Module(const std::string& moduleFileanme);
        ~Win32Module();

        Win32Module(const Win32Module&) = delete;
        Win32Module& operator = (const Win32Module&) = delete;

        void* LoadProcedure(const std::string& procedureName) override;

    private:

        HMODULE handle_ = 0;

};


} // /namespace Ac


#endif



// ================================================================================
