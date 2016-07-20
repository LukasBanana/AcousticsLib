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


ALBufferObjQueue::~ALBufferObjQueue()
{
    alDeleteBuffers(static_cast<ALsizei>(handles_.size()), handles_.data());
}

void ALBufferObjQueue::QueueBufferData(ALuint sourceHandle, ALenum format, const ALvoid* buffer, ALsizei size, ALsizei sampleRate)
{
    /* Select buffer handle */
    ALuint bufferHandle = 0;

    ALint processed = 0;
    alGetSourcei(sourceHandle, AL_BUFFERS_PROCESSED, &processed);

    if (processed > 0)
    {
        /* Pop oldest buffer from queue and push it back with new data */
        alSourceUnqueueBuffers(sourceHandle, 1, &bufferHandle);
    }
    else
    {
        /* Use new buffer object until the source queue is long enough */
        bufferHandle = AllocBuffer();
    }

    /* Fill buffer object with wave buffer data */
    alBufferData(bufferHandle, format, buffer, size, sampleRate);

    /* Push buffer to the end of the source buffer queue */
    alSourceQueueBuffers(sourceHandle, 1, &bufferHandle);
}

void ALBufferObjQueue::QueueBufferData(ALuint sourceHandle, const WaveBuffer& waveBuffer)
{
    ALenum fmt = 0;
    if (ALFormatFromWaveFormat(fmt, waveBuffer.GetFormat()))
    {
        QueueBufferData(
            sourceHandle,
            fmt,
            waveBuffer.Data(),
            waveBuffer.BufferSize(),
            waveBuffer.GetFormat().sampleRate
        );
    }
}


/*
 * ======= Private: =======
 */

ALuint ALBufferObjQueue::AllocBuffer()
{
    ALuint handle = 0;

    alGenBuffers(1, &handle);
    if (alGetError() != AL_NO_ERROR)
        throw std::runtime_error("failed to generate OpenAL buffer object queue");

    handles_.push_back(handle);
    return handle;
}


} // /namespace Ac



// ================================================================================
