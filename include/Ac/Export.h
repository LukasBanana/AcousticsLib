/*
 * Export.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_EXPORT_H
#define AC_EXPORT_H


#ifdef _MSC_VER
#   define AC_EXPORT __declspec(dllexport)
#else
#   define AC_EXPORT
#endif


#endif



// ================================================================================
