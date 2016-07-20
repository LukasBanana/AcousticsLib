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

        std::unique_ptr<Sound> CreateSound() override;
        std::unique_ptr<Sound3D> CreateSound3D() override;

        void SetListenerPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetListenerPosition() const override;

        void SetListenerVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetListenerVelocity() const override;

        void SetListenerOrientation(const ListenerOrientation& orientation) override;
        ListenerOrientation GetListenerOrientation() const override;

    private:

        std::vector<std::string> ListDevices() const;

        ALCdevice* OpenDevice();
        ALCcontext* CreateContext();

        ALCdevice*  device_     = nullptr;
        ALCcontext* context_    = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
