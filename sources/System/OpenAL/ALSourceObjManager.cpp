/*
 * ALSourceObjManager.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALSourceObjManager.h"
#include "ALSound.h"


namespace Ac
{


static ALSourceObjManager* g_instanceRef = nullptr;

ALSourceObjManager::ALSourceObjManager(std::size_t maxHandles) :
    maxHandles_ { maxHandles }
{
    g_instanceRef = this;
}

ALSourceObjManager::~ALSourceObjManager()
{
    g_instanceRef = nullptr;
}

ALSourceObjManager& ALSourceObjManager::Instance()
{
    return *g_instanceRef;
}

ALSourceObj* ALSourceObjManager::AllocSourceObj(ALSound& snd)
{
    if (usedHandles_ < maxHandles_)
    {
        ALSourceObj* sourceObj = nullptr;

        if (usedHandles_ < handles_.size())
        {
            /* Reuse allocated source object */
            handles_[usedHandles_].associatedSound = &snd;
            sourceObj = handles_[usedHandles_].source.get();
        }
        else
        {
            /* Create new source object */
            handles_.push_back({ std::unique_ptr<ALSourceObj>(new ALSourceObj()), &snd });
            sourceObj = handles_.back().source.get();
        }

        /* Increase usage counter */
        ++usedHandles_;

        return sourceObj;
    }
    return nullptr;
}

void ALSourceObjManager::FreeSourceObj(ALSourceObj* sourceObj)
{
    for (std::size_t i = 0; i < usedHandles_; ++i)
    {
        auto& handle = handles_[i];

        if (handle.source.get() == sourceObj)
        {
            /* Drop source object on sound */
            if (auto& snd = handle.associatedSound)
            {
                snd->DropSourceObj();
                snd = nullptr;
            }

            /* Reduce used handles */
            --usedHandles_;

            /* Move current handle to the end of the list */
            if (usedHandles_ != i)
                std::swap(handles_[i], handles_[usedHandles_]);

            break;
        }
    }
}


} // /namespace Ac



// ================================================================================
