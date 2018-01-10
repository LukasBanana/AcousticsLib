/*
 * ComPtr.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_COM_PTR_H
#define AC_COM_PTR_H


#include <wrl.h>


namespace Ac
{


// Type alias for Win32 ComPtr template
template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;


} // /namespace Ac


#endif



// ================================================================================
