/*
 * XA2AudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_XA2_AUDIO_SYSTEM_H
#define AC_XA2_AUDIO_SYSTEM_H


#include <Ac/AudioSystem.h>


namespace Ac
{


class XA2AudioSystem : public AudioSystem
{

    public:

        std::string GetVersion() const override;

        std::unique_ptr<Sound> LoadSound(const std::string& filename) override;
        std::unique_ptr<Sound3D> LoadSound3D(const std::string& filename) override;

        void SetListenerPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetListenerPosition() const override;

        void SetListenerVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetListenerVelocity() const override;

        void SetListenerOrientation(const ListenerOrientation& orientation) override;
        ListenerOrientation GetListenerOrientation() const override;

};


} // /namespace Ac


#endif



// ================================================================================
