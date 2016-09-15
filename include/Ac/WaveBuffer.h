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
#include <functional>


namespace Ac
{


//! Raw audio PCM (Pulse Modulation Code) buffer type.
using PCMBuffer = std::vector<char>;

/**
\brief Interface of the iteration callback function to iterate over wave buffer samples.
\param[in,out] sample Specifies the current sample which is to be modified. Each sample will be clamped to the range [-1, 1].
\param[in] channel Specifies the current channel to which the sample belongs.
\param[in] index Specifies the current sample index.
\param[in] timePoint Specifies the current timePoint within the entire wave buffer (in seconds).
\remarks This function interface is used for the 'ForEachSample' function.
\see WaveBuffer::ForEachSample
*/
using SampleIterationFunction = std::function<void(double& sample, unsigned short channel, std::size_t index, double timePoint)>;

/**
\brief Interface of the constant iteration callback function to iterate over wave buffer samples.
\param[in] sample Specifies the current sample which is to be read.
\param[in] channel Specifies the current channel to which the sample belongs.
\param[in] index Specifies the current sample index.
\param[in] timePoint Specifies the current timePoint within the entire wave buffer (in seconds).
\remarks This function interface is used for the 'ForEachSample' function.
\see WaveBuffer::ForEachSample
*/
using SampleConstIterationFunction = std::function<void(double sample, unsigned short channel, std::size_t index, double timePoint)>;


/**
\brief Data model for an audio wave buffer.
\remarks This class manages the PCM (Pulse Code Modulation) buffer by abstracting the underlying audio samples
(8 or 16 bit, signed or unsigned) to double precision floating-points in the normalized range [-1, 1].
Here is a usage example:
\code
// Create wave buffer with 44 kHz sample rate, 16-bit samples, and two channels
Ac::WaveBuffer buffer(WaveBufferFormat(Ac::Synthesizer::sampleRate44kHz, 16, 2));

// Allocate internal buffer to store samples for 4.5 seconds.
buffer.SetTotalTime(4.5);

// Generate samples for the first half of the wave buffer (here a sine wave of 350 Hz)
buffer.ForEachSample(Ac::Synthesizer::SineGenerator(0.3, 0.0, 350.0), 0.0, 2.25);

// Generate sample for the second half of the wave buffer (here with a custom function)
buffer.ForEachSample(
    [](double& sample, unsigned short channel, std::size_t index, double timePoint)
    {
        //sample += ...
    },
    2.25, 4.5
);

// Now create sound with our buffer
auto sound = audioSystem->CreateSound(buffer);
sound->Play();
\endcode
*/
class AC_EXPORT WaveBuffer
{

    public:

        WaveBuffer() = default;
        WaveBuffer(const WaveBuffer&) = default;
        WaveBuffer& operator = (const WaveBuffer&) = default;

        WaveBuffer(const WaveBufferFormat& format);
        WaveBuffer(WaveBuffer&& other);
        WaveBuffer& operator = (WaveBuffer&& other);

        //! Returns the number of samples per channel.
        std::size_t GetSampleFrames() const;

        //! Sets the new number of samples per channel.
        void SetSampleFrames(std::size_t sampleFrames);

        //! Returns the total time (in seconds) which is required to play this entire wave buffer.
        double GetTotalTime() const;

        //! Resizes the buffer to the specified total time (in seconds).
        void SetTotalTime(double duration);

        /**
        \brief Returns the sample at the specified index of the specified channel.
        \param[in] index Specifies the sample index. One can use the "GetIndexFromTimePoint" function, to determine the index by the time point (in seconds).
        \param[in] channel Specifies the channel from which to read the sample. This will be clamped to the range [0, GetFormat().channels).
        \return The read sample in the range [-1, 1].
        \see GetIndexFromTimePoint
        */
        double ReadSample(std::size_t index, unsigned short channel) const;

        //! Sets the sample at the specified index of the specified channel.
        void WriteSample(std::size_t index, unsigned short channel, double sample);

        //! Returns the sample at the specified time point of the specified channel.
        double ReadSample(double timePoint, unsigned short channel) const;

        //! Sets the sample at the specified time point of the specified channel.
        void WriteSample(double timePoint, unsigned short channel, double sample);

        /**
        \brief Determines the sample index for the specified time point (in seconds).
        \see ReadSample(std::size_t, unsigned short) const
        \see WriteSample(std::size_t, unsigned short, double)
        \see GetTimePointFromIndex
        */
        std::size_t GetIndexFromTimePoint(double timePoint) const;

        /**
        \brief Determines the time point (in seconds) for the specified sample index.
        \see GetIndexFromTimePoint
        */
        double GetTimePointFromIndex(std::size_t index) const;

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

        /**
        \brief Swaps the endianness (byte order) of each sample between little-endian and big-endian.
        \remarks Per default, all data is read in little endian format on an x86 (IA-32) and x64 (AMD64) processor.
        Therefore, calling this function swaps the byte order to big-endian. This is used for the AIFF reader for instance.
        If the bits per sample is 8 (or lower), then this function has no effect, since byte order does not matter for a single byte.
        */
        void SwapEndianness();

        /* ----- Sample iteration ----- */
        
        /**
        \brief Iterates over all samples of this wave buffer within the specified range.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \param[in] indexBegin Specifies the first sample index.
        \param[in] indexEnd Specifies the last sample index. The ending is inclusive, i.e. the iteration range is [indexBegin, indexEnd].
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleIterationFunction& iterator, std::size_t indexBegin, std::size_t indexEnd);

        /**
        \brief Iterates over all samples of this wave buffer within the specified time range.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \param[in] timeBegin Specifies the beginning time point (in seconds). This will be clamped to [0, +inf).
        \param[in] timeEnd Specifies the ending time point (in seconds). This will be clamped to [timeBegin, +inf).
        The ending is inclusive, i.e. the iteration range is [timeBegin, timeEnd].
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleIterationFunction& iterator, double timeBegin, double timeEnd);

        /**
        \brief Iterates over all samples of this wave buffer.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleIterationFunction& iterator);
    
        /**
        \brief Iterates over all samples of this wave buffer within the specified range with a constant iterator.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \param[in] indexBegin Specifies the first sample index.
        \param[in] indexEnd Specifies the last sample index. The ending is inclusive, i.e. the iteration range is [indexBegin, indexEnd].
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleConstIterationFunction& iterator, std::size_t indexBegin, std::size_t indexEnd) const;
    
        /**
        \brief Iterates over all samples of this wave buffer within the specified time range with a constant iterator.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \param[in] timeBegin Specifies the beginning time point (in seconds). This will be clamped to [0, +inf).
        \param[in] timeEnd Specifies the ending time point (in seconds). This will be clamped to [timeBegin, +inf).
        The ending is inclusive, i.e. the iteration range is [timeBegin, timeEnd].
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleConstIterationFunction& iterator, double timeBegin, double timeEnd) const;
    
        /**
        \brief Iterates over all samples of this wave buffer with a constant iterator.
        \param[in] iterator Specifies the sample iteration callback function. This function will be used to modify each sample.
        \see SampleIterationFunction
        */
        void ForEachSample(const SampleConstIterationFunction& iterator) const;

        /* ----- Appending ----- */

        /**
        \brief Appends the specified wave buffer to this buffer.
        \param[in] buffer Specifies the new wave buffer which is to be appended to this buffer.
        The format of the input buffer will be set to the format of this buffer (if they are unequal).
        \remarks The input buffer must not be the same as this buffer, i.e. 'buffer.Append(buffer)' is an invalid operation and the behavior is undefined!
        \see SetFormat
        */
        void Append(const WaveBuffer& other);

        /* ----- Copying ----- */

        /**
        \brief Copyies a portion of the specified source buffer into this buffer.
        \param[in] source Specifies the source buffer to copy from.
        \param[in] indexBegin Specifies the first sample index of the source buffer.
        \param[in] indexEnd Specifies the last sample index of the source buffer. The ending is exclusive, i.e. the range is [indexBegin, indexEnd).
        \param[in] destIndexOffset Specifies the destination index offset.
        */
        void CopyFrom(const WaveBuffer& source, std::size_t indexBegin, std::size_t indexEnd, std::size_t destIndexOffset);

        /**
        \brief Copyies a portion of the specified source buffer into this buffer.
        \param[in] source Specifies the source buffer to copy from.
        \param[in] timeBegin Specifies the beginning time point (in seconds) of the source buffer. This will be clamped to [0, +inf).
        \param[in] timeEnd Specifies the ending time point (in seconds) of the source buffer. This will be clamped to (timeBegin, +inf).
        The ending is exclusive, i.e. the range is [timeBegin, timeEnd).
        \param[in] destTimeOffset Specifies the destination time offset (in seconds).
        \see CopyFrom(const WaveBuffer&, std::size_t, std::size_t, std::size_t)
        */
        void CopyFrom(const WaveBuffer& source, double timeBegin, double timeEnd, double destTimeOffset);

        /**
        \brief Copies the entire source buffer into this buffer.
        \param[in] source Specifies the source buffer to copy from.
        \param[in] destIndexOffset Specifies the destination index offset.
        \see CopyFrom(const WaveBuffer&, std::size_t, std::size_t, std::size_t)
        */
        void CopyFrom(const WaveBuffer& source, std::size_t destIndexOffset);

        /**
        \brief Copies the entire source buffer into this buffer.
        \param[in] source Specifies the source buffer to copy from.
        \param[in] destTimeOffset Specifies the destination time offset (in seconds).
        \see CopyFrom(const WaveBuffer&, std::size_t)
        */
        void CopyFrom(const WaveBuffer& source, double destTimeOffset);

        /* ----- Raw buffer access ----- */

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

        std::size_t GetPCMBufferOffset(std::size_t index, unsigned short channel) const;

        char* GetPCMOffsetPtr(std::size_t offset);
        const char* GetPCMOffsetPtr(std::size_t offset) const;

        WaveBufferFormat    format_;

        PCMBuffer           buffer_;

};


} // /namespace Ac


#endif



// ================================================================================
