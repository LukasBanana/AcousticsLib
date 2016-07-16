/*
 * XA2AudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_XA2_AUDIO_SYSTEM_H__
#define __AC_XA2_AUDIO_SYSTEM_H__


#include <Ac/AudioSystem.h>


namespace Ac
{


class XA2AudioSystem : public AudioSystem
{

    public:

        std::string GetVersion() const override;

        std::unique_ptr<Sound> LoadSound(const std::string& filename) override;

};


} // /namespace Ac


#endif



// ================================================================================
