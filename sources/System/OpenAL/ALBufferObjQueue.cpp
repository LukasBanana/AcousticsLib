/*
 * ALBufferObjQueue.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ALBufferObjQueue.h"
#include "ALFormat.h"


namespace Ac
{


ALBufferObjQueue::ALBufferObjQueue(ALuint sourceHandle) :
    sourceHandle_ { sourceHandle }
{
}

ALBufferObjQueue::~ALBufferObjQueue()
{
    if (!handles_.empty())
        alDeleteBuffers(static_cast<ALsizei>(handles_.size()), handles_.data());
}

void ALBufferObjQueue::Reset()
{
    /* Reset processed time */
    processedTime_ = 0.0;

    /* Unqueue all buffers */
    /*ALint queued = 0;
    alGetSourcei(sourceHandle_, AL_BUFFERS_QUEUED, &queued);
    alSourceUnqueueBuffers(sourceHandle_, queued, nullptr);*/
}

void ALBufferObjQueue::QueueBufferData(const WaveBuffer& waveBuffer)
{
    /* Get wave buffer attributes */
    auto size = static_cast<ALsizei>(waveBuffer.BufferSize());
    auto sampleRate = static_cast<ALsizei>(waveBuffer.GetFormat().sampleRate);

    ALenum format = 0;
    if (!ALFormatFromWaveFormat(format, waveBuffer.GetFormat()))
        return;
    
    /* Select buffer handle */
    ALuint bufferHandle = 0;

    ALint processed = 0;
    alGetSourcei(sourceHandle_, AL_BUFFERS_PROCESSED, &processed);

    if (processed > 0)
    {
        /* Pop oldest buffer from queue and push it back with new data */
        alSourceUnqueueBuffers(sourceHandle_, 1, &bufferHandle);

        /* Accumulate processed total time */
        processedTime_ += totalTimes_.front();
        totalTimes_.pop();
    }
    else
    {
        /* Use new buffer object until the source queue is long enough */
        bufferHandle = AllocBuffer();
    }

    /* Fill buffer object with wave buffer data */
    alBufferData(bufferHandle, format, waveBuffer.Data(), size, sampleRate);

    /* Push buffer to the end of the source buffer queue */
    alSourceQueueBuffers(sourceHandle_, 1, &bufferHandle);

    totalTimes_.push(waveBuffer.GetTotalTime());
}


/*
 * ======= Private: =======
 */

ALuint ALBufferObjQueue::AllocBuffer()
{
    ALuint handle = 0;

    alGenBuffers(1, &handle);

    auto err = alGetError();
    if (err != AL_NO_ERROR)
        throw std::runtime_error("failed to generate OpenAL buffer object queue (" + ALErrorToString(err) + ")");

    handles_.push_back(handle);
    return handle;
}


} // /namespace Ac



// ================================================================================
