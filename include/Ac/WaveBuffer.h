/*
 * WaveBuffer.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_WAVE_BUFFER_H__
#define __AC_WAVE_BUFFER_H__


#include "Export.h"
#include "WaveBufferFormat.h"

#include <vector>
#include <queue>
#include <memory>


namespace Ac
{


//! Raw audio PCM (Pulse Modulation Code) buffer type.
using PCMBuffer = std::vector<char>;


//! Data model structure for an audio wave buffer.
class AC_EXPORT WaveBuffer
{

    public:

        WaveBuffer() = default;
        WaveBuffer(const WaveBufferFormat& format);

        //! Sets the new number of samples (independently of the number of channels).
        void SetSampleCount(std::size_t sampleCount);

        //! Returns the number of samples (independently of the number of channels).
        std::size_t GetSampleCount() const;

        //! Resizes the buffer to the specified total time (in seconds).
        void SetTotalTime(double duration);

        //! Returns the total time (in seconds) which is required to play this entire wave buffer.
        double GetTotalTime() const;

        //! Returns the sample at the specified time point (phase) of the specified channel.
        double ReadSample(double phase, unsigned short channel) const;

        //! Sets the sample at the specified time point (phase) of the specified channel.
        void WriteSample(double phase, unsigned short channel, double sample);

        /**
        \brief Sets the new wave buffer format.
        \param[in] format Specifies the new wave buffer format. If this is equal to the previous buffer, this function has no effect.
        \remarks This function may take some computational overhead, since the entire PCM buffer needs to be resampled.
        */
        void SetFormat(const WaveBufferFormat& format);

        /**
        \brief Sets the new number of channels. By default 2.
        \remarks This is a shortcut for the following behavior:
        \code
        auto format = this->GetFormat();
        format.channels = channels;
        this->SetFormat(format);
        \endcode
        \see SetFormat
        */
        void SetChannels(unsigned short channels);

        //! Returns the actual PCM buffer size (in bytes).
        inline std::size_t BufferSize() const
        {
            return buffer_.size();
        }

        //! Returns a raw pointer to the PCM buffer data.
        inline char* Data()
        {
            return buffer_.data();
        }

        //! Returns a constant raw pointer to the PCM buffer data.
        inline const char* Data() const
        {
            return buffer_.data();
        }

        //! Returns the format description of this wave buffer.
        inline const WaveBufferFormat& GetFormat() const
        {
            return format_;
        }

    private:

        std::size_t GetPCMBufferOffset(double phase, unsigned short channel) const;

        void* GetPCMOffsetPtr(double phase, unsigned short channel);
        const void* GetPCMOffsetPtr(double phase, unsigned short channel) const;

        WaveBufferFormat    format_;

        PCMBuffer           buffer_;

};


} // /namespace Ac


#endif



// ================================================================================
