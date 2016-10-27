/*
 * LinuxModule.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_LINUX_MODULE_H
#define AC_LINUX_MODULE_H


#include "../Module.h"


namespace Ac
{


class LinuxModule : public Module
{

    public:

        LinuxModule(const std::string& moduleFilename);
        ~LinuxModule();

        LinuxModule(const LinuxModule&) = delete;
        LinuxModule& operator = (const LinuxModule&) = delete;

        void* LoadProcedure(const std::string& procedureName) override;

    private:

        void* handle_ = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
