/*
 * PackPush.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* <--- THIS FILE MUST NOT HAVE A HEADER GUARD! ---> */

#ifdef AC_PACK_STRUCT
#   error AC_PACK_STRUCT already defined: "PackPush.h" included but no succeeding "PackPop.h"!
#endif

#if defined(_MSC_VER)
#   pragma pack(push, packing)
#   pragma pack(1)
#   define AC_PACK_STRUCT
#elif defined(__GNUC__) || defined(__clang__)
#   define AC_PACK_STRUCT __attribute__((packed))
#else
#   error Compiler not supported
#endif



// ================================================================================
