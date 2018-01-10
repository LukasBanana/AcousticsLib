/*
 * XA2AudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_XA2_AUDIO_SYSTEM_H
#define AC_XA2_AUDIO_SYSTEM_H


#include <Ac/AudioSystem.h>
#include <xaudio2.h>
#include "ComPtr.h"


namespace Ac
{


class XA2AudioSystem : public AudioSystem
{

    public:

        XA2AudioSystem();
        ~XA2AudioSystem();

        std::string GetVersion() const override;

        std::unique_ptr<Sound> CreateSound() override;

        void SetListenerPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetListenerPosition() const override;

        void SetListenerVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetListenerVelocity() const override;

        void SetListenerOrientation(const ListenerOrientation& orientation) override;
        ListenerOrientation GetListenerOrientation() const override;

    private:

        ComPtr<IXAudio2>        device_;
        IXAudio2MasteringVoice* masteringVoice_ = nullptr;

};


} // /namespace Ac


#endif



// ================================================================================
