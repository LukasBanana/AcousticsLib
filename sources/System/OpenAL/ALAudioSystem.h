/*
 * ALAudioSystem.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_AUDIO_SYSTEM_H__
#define __AC_AL_AUDIO_SYSTEM_H__


#include "OpenAL.h"
#include "ALBufferObj.h"

#include <Ac/AudioSystem.h>


namespace Ac
{


class ALAudioSystem : public AudioSystem
{

    public:

        ALAudioSystem();
        ~ALAudioSystem();

        std::string GetVersion() const override;

        std::unique_ptr<Sound> LoadSound(const std::string& filename) override;

    private:

        ALCdevice* OpenDevice();
        ALCcontext* CreateContext();

        std::unique_ptr<ALBufferObj> CreateBufferObjFromFile(const std::string& filename);

        ALCdevice*  device_     = nullptr;
        ALCcontext* context_    = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
