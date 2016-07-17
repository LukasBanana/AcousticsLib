/*
 * MacOSModule.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_MACOS_MODULE_H__
#define __AC_MACOS_MODULE_H__


#include "../Module.h"


namespace Ac
{


class MacOSModule : public Module
{

    public:

        MacOSModule(const std::string& moduleFilename);
        MacOSModule();

        MacOSModule(const MacOSModule&) = delete;
        MacOSModule& operator = (const MacOSModule&) = delete;

        void* LoadProcedure(const std::string& procedureName) override;

    private:

        void* handle_ = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
