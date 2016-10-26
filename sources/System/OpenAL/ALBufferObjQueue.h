/*
 * ALBufferObjQueue.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_BUFFER_OBJ_QUEUE_H
#define AC_AL_BUFFER_OBJ_QUEUE_H


#include "OpenAL.h"

#include <Ac/WaveBuffer.h>
#include <vector>
#include <queue>


namespace Ac
{


class ALBufferObjQueue
{

    public:

        ALBufferObjQueue(ALuint sourceHandle);
        ~ALBufferObjQueue();

        void Reset();

        void QueueBufferData(const WaveBuffer& waveBuffer);

        inline std::size_t QueueSize() const
        {
            return handles_.size();
        }

        inline double ProcessedTime() const
        {
            return processedTime_;
        }

    private:

        ALuint AllocBuffer();

        ALuint              sourceHandle_;
        std::vector<ALuint> handles_;
        std::queue<double>  totalTimes_;
        double              processedTime_ = 0.0;

};


} // /namespace Ac


#endif



// ================================================================================
