/*
 * ALAudioSystem.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_AUDIO_SYSTEM_H__
#define __AC_AL_AUDIO_SYSTEM_H__


#include <Ac/AudioSystem.h>
#include <AL/al.h>
#include <AL/alc.h>


namespace Ac
{


class ALAudioSystem : public AudioSystem
{

    public:

        ALAudioSystem();
        ~ALAudioSystem();

        std::string GetVersion() const override;

    private:

        ALCdevice* OpenDevice();
        ALCcontext* CreateContext();

        ALCdevice*  device_     = nullptr;
        ALCcontext* context_    = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
