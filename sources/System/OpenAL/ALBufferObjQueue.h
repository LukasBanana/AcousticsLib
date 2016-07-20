/*
 * ALBufferObjQueue.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_BUFFER_OBJ_QUEUE_H__
#define __AC_AL_BUFFER_OBJ_QUEUE_H__


#include "OpenAL.h"

#include <Ac/WaveBuffer.h>
#include <vector>


namespace Ac
{


class ALBufferObjQueue
{

    public:

        ~ALBufferObjQueue();

        void QueueBufferData(ALuint sourceHandle, ALenum format, const ALvoid* buffer, ALsizei size, ALsizei sampleRate);
        void QueueBufferData(ALuint sourceHandle, const WaveBuffer& waveBuffer);

        inline std::size_t QueueSize() const
        {
            return handles_.size();
        }

    private:

        ALuint AllocBuffer();

        std::vector<ALuint> handles_;

};


} // /namespace Ac


#endif



// ================================================================================
