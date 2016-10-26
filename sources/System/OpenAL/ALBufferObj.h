/*
 * ALBufferObj.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AL_BUFFER_OBJ_H
#define AC_AL_BUFFER_OBJ_H


#include "OpenAL.h"

#include <Ac/WaveBuffer.h>
#include <Ac/AudioSystem.h>


namespace Ac
{


class ALBufferObj
{

    public:

        ALBufferObj();
        ALBufferObj(const WaveBuffer& waveBuffer);
        ~ALBufferObj();

        void BufferData(ALenum format, const ALvoid* buffer, ALsizei size, ALsizei sampleRate);
        void BufferData(const WaveBuffer& waveBuffer);

        //! Returns the handle of this buffer object.
        inline ALuint Get() const
        {
            return handle_;
        }

        //! Returns the total time (in seconds) of the buffer.
        inline double GetTotalTime() const
        {
            return totalTime_;
        }

    private:

        ALuint handle_      = 0;

        double totalTime_   = 0.0;

};


} // /namespace Ac


#endif



// ================================================================================
