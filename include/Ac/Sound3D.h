/*
 * Sound3D.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_SOUND3D_H__
#define __AC_SOUND3D_H__


#include <Gauss/Vector3.h>

#include "Sound.h"


namespace Ac
{


//! 3D sound source interface.
class AC_EXPORT Sound3D : public Sound
{

    public:

        Sound3D(const Sound3D&) = delete;
        Sound3D& operator = (const Sound3D&) = delete;
        
        //! Sets the world position of this 3D sound. By default (0, 0, 0).
        virtual void SetPosition(const Gs::Vector3f& position) = 0;
        //! Returns the world position of this 3D sound.
        virtual Gs::Vector3f GetPosition() const = 0;

        //! Sets the world velocity of this 3D sound. The velocity is used for the "Doppler"-effect. By default (0, 0, 0).
        virtual void SetVelocity(const Gs::Vector3f& velocity) = 0;
        //! Returns the world velocity of this 3D sound.
        virtual Gs::Vector3f GetVelocity() const = 0;

        //! Specifies whether to make the coordinate space of this sound relative to the listener or not. By default false.
        virtual void SetSpaceRelative(bool enable) = 0;
        //! Returns true if the coordinate space of this sound is relative to the listener or not.
        virtual bool GetSpaceRelative() const = 0;

    protected:

        Sound3D() = default;

};


} // /namespace Ac


#endif



// ================================================================================
