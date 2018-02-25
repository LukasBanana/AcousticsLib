/*
 * ALSourceObj.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_SOURCE_OBJ_H
#define AC_AL_SOURCE_OBJ_H


#include "OpenAL.h"
#include "ALBufferObj.h"

#include <Gauss/Vector3.h>
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

        void SetInt(ALenum param, ALint value);
        ALint GetInt(ALenum param) const;

        void SetFloat(ALenum param, ALfloat value);
        ALfloat GetFloat(ALenum param) const;

        void SetVector3(ALenum param, const Gs::Vector3f value);
        Gs::Vector3f GetVector3(ALenum param) const;

        // Returns the handle of this source object.
        inline ALuint Get() const
        {
            return handle_;
        }

        // Returns the number of source objects.
        static std::size_t GetSourceCount();

    private:

        static std::size_t sourceCounter_;

        ALuint handle_ = 0;

};


} // /namespace Ac


#endif



// ================================================================================
