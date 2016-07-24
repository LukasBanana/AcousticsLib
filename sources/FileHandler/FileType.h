/*
 * FileType.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_FILE_TYPE_H__
#define __AC_FILE_TYPE_H__


#include <Ac/AudioFormats.h>
#include <istream>


namespace Ac
{


AudioFormats DetermineAudioFormat(std::istream& stream);


} // /namespace Ac


#endif



// ================================================================================
