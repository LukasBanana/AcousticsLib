/*
 * ALSourceObj.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_SOURCE_OBJ_H__
#define __AC_AL_SOURCE_OBJ_H__


#include "OpenAL.h"
#include "ALBufferObj.h"

#include <Ac/AudioSystem.h>


namespace Ac
{


class ALSourceObj
{

    public:

        ALSourceObj();
        ~ALSourceObj();

        void AttachBuffer(const ALBufferObj& bufferObj);
        void DetachBuffer();

        //! Returns the value of AL_SOURCE_STATE for this source object.
        ALint GetState() const;

        //! Returns the handle of this source object.
        inline ALuint Get() const
        {
            return handle_;
        }

    private:

        ALuint handle_ = 0;

};


} // /namespace Ac


#endif



// ================================================================================
