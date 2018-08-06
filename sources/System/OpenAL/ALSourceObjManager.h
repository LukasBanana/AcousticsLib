/*
 * ALSourceObjManager.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_SOURCE_OBJ_MANAGER_H
#define AC_AL_SOURCE_OBJ_MANAGER_H


#include "ALSourceObj.h"
#include <memory>
#include <vector>


namespace Ac
{


class ALSound;

class ALSourceObjManager
{

    public:

        ALSourceObjManager(std::size_t maxHandles);
        ~ALSourceObjManager();

        static ALSourceObjManager& Instance();

        ALSourceObj* AllocSourceObj(ALSound& snd);
        void FreeSourceObj(ALSourceObj* sourceObj);

    private:

        struct SourceHandle
        {
            std::unique_ptr<ALSourceObj>    source;
            ALSound*                        associatedSound;
        };

        std::vector<SourceHandle>   handles_;
        std::size_t                 maxHandles_     = 0;
        std::size_t                 usedHandles_    = 0;

};


} // /namespace Ac


#endif



// ================================================================================
