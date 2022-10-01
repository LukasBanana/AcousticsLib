/*
 * ALAudioSystem.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_AUDIO_SYSTEM_H
#define AC_AL_AUDIO_SYSTEM_H


#include "OpenAL.h"
#include "ALBufferObj.h"
#include "ALSourceObjManager.h"

#include <Ac/AudioSystem.h>
#include <vector>
#include <memory>


namespace Ac
{


class ALAudioSystem : public AudioSystem
{

    public:

        ALAudioSystem();
        ~ALAudioSystem();

        std::string GetVersion() const override;
        AudioLimitations GetLimits() const override;

        std::unique_ptr<Sound> CreateSound() override;

        void SetListenerPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetListenerPosition() const override;

        void SetListenerVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetListenerVelocity() const override;

        void SetListenerOrientation(const ListenerOrientation& orientation) override;
        ListenerOrientation GetListenerOrientation() const override;

    private:

        struct ContextAttributes
        {
            ALCint frequency        = 25;
            ALCint numMonoSources   = 16;
            ALCint numStereoSources = 16;
        };

        std::vector<std::string> ListDevices() const;

        ALCdevice* OpenDevice();
        ALCcontext* CreateContext();

        ContextAttributes QueryAttributes();

        ALCdevice*          device_         = nullptr;
        ALCcontext*         context_        = nullptr;
        ContextAttributes   contextAttribs_;
        ALSourceObjManager  sourceObjMngr_;

};


} // /namespace Ac


#endif



// ================================================================================
