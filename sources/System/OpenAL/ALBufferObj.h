/*
 * ALBufferObj.h
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AL_BUFFER_OBJ_H__
#define __AC_AL_BUFFER_OBJ_H__


#include "OpenAL.h"
#include "../../Core/WaveBuffer.h"

#include <Ac/AudioSystem.h>


namespace Ac
{


class ALBufferObj
{

    public:

        ALBufferObj();
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
