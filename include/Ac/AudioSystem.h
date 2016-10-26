/*
 * AudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_AUDIO_SYSTEM_H
#define AC_AUDIO_SYSTEM_H


#include "Export.h"
#include "Sound.h"
#include "AudioStream.h"
#include "AudioFormats.h"
#include "Microphone.h"

#include <Gauss/Vector3.h>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <thread>
#include <mutex>


namespace Ac
{


//! Structure for the 3D listener orientation with at- and up- vectors.
struct AC_EXPORT ListenerOrientation
{
    Gs::Vector3f atVector;
    Gs::Vector3f upVector;
};

//! Loading sound flags enumeration.
struct SoundFlags
{
    using BitMask = unsigned int;
    enum
    {
        //! Indicates that "LoadSound" shall always return a valid Sound object, even if the sound file could not be loaded.
        AlwaysCreateSound   = (1 << 0),

        /**
        \brief Indicates that "LoadSound" shall return a Sound object which is prepared for the 3D sound features.
        \see Sound::Enable3D
        */
        Enable3D            = (1 << 1),
    };
};


/**
\brief Audio system interface.
\remarsk All coordinates or 3D sounds are meant to be in a left-handed coordinates system,
i.e. positive Z values point into your monitor, and negative Z values point out of your monitor.
*/
class AC_EXPORT AudioSystem
{

    public:

        /* ----- Audio system ----- */

        AudioSystem(const AudioSystem&) = delete;
        AudioSystem& operator = (const AudioSystem&) = delete;

        virtual ~AudioSystem();
    
        /**
        \brief Returns the list of all available audio system modules for the current platform
        (e.g. on Windows this might be { "OpenAL", "XAudio2" }, but on MacOS it might be only { "OpenAL" }).
        */
        static std::vector<std::string> FindModules();

        /**
        \brief Loads a new audio system from the specified module.
        \param[in] moduleName Specifies the name from which the new audio system is to be loaded.
        This denotes a dynamic library (*.dll-files on Windows, *.so-files on Unix systems).
        If compiled in debug mode, the postfix "D" is appended to the module name.
        Moreover, the platform dependent file extension is always added automatically
        as well as the prefix "AcLib_", i.e. a module name "OpenAL"
        will be translated to "AcLib_OpenALD.dll", if compiled on Windows in Debug mode.
        \remarks Usually the return type is a std::unique_ptr, but the AcousticsLib needs to keep track
        of the existance of this audio system because only a single instance can be loaded at a time.
        So a std::weak_ptr is stored internally to check if it has been expired
        (see http://en.cppreference.com/w/cpp/memory/weak_ptr/expired),
        and this type can only refer to a std::shared_ptr.
        \throws std::runtime_exception If loading the audio system from the specified module failed.
        \throws std::runtime_exception If there is already a loaded instance of an audio system
        (make sure there are no more shared pointer references to the previous audio system!)
        */
        static std::shared_ptr<AudioSystem> Load(const std::string& moduleName);

        //! Returns the first available audio system.
        static std::shared_ptr<AudioSystem> Load();

        //! Returns the name of this audio system.
        inline const std::string& GetName() const
        {
            return name_;
        }

        //! Returns a descriptive version string of this audio system (e.g. "OpenAL 1.1").
        virtual std::string GetVersion() const = 0;

        /* ----- Sounds ----- */

        //! Creates an empty sound which can later be filled with a wave buffer.
        virtual std::unique_ptr<Sound> CreateSound() = 0;

        //! Creates a sound initialized with specified wave buffer.
        std::unique_ptr<Sound> CreateSound(const WaveBuffer& waveBuffer);

        /**
        \brief Loads the specified sound from file.
        \param[in] flags Specifies the bit mask flags.
        This can be a bitwas OR combination of the values of the "SoundFlags" enumeration. By default 0.
        \see SoundFlags
        */
        std::unique_ptr<Sound> LoadSound(const std::string& filename, const SoundFlags::BitMask flags = 0);

        /**
        \brief Plays directly the specified sound file.
        \param[in] filename Specifies the sound file to play.
        \param[in] volume Specifies the volume. By default 1.
        \param[in] pitch Specifies the pitch (or frequency multiplier). By default 1.
        */
        void Play(const std::string& filename, float volume = 1.0f, float pitch = 1.0f);

        /**
        \brief Performs the audio streaming process. This should be called once per frame in a real-time application.
        \param[in,out] sound Specifies the sound for which the streaming is to be performed.
        \param[in,out] waveBuffer Specifies the wave buffer for buffering during the streaming process.
        If the buffer is empty, an appropriate buffer will be configured for streaming.
        \remarks If the specified sound does not have a source stream, this function call has no effect.
        \see Sound::SetSourceStream
        */
        void Streaming(Sound& sound, WaveBuffer& waveBuffer);

        /**
        \brief Performs the audio streaming process with a default wave buffer configuration.
        \see Streaming(Sound&, WaveBuffer&)
        */
        void Streaming(Sound& sound);

        /* ----- Listener ----- */

        //! Sets the listener world position. By default (0, 0, 0).
        virtual void SetListenerPosition(const Gs::Vector3f& position) = 0;

        //! Returns the listener world position.
        virtual Gs::Vector3f GetListenerPosition() const = 0;

        //! Sets the listener world velocity. This is used for the "Doppler"-effect. By default (0, 0, 0).
        virtual void SetListenerVelocity(const Gs::Vector3f& velocity) = 0;
        
        //! Returns the listener world velocity.
        virtual Gs::Vector3f GetListenerVelocity() const = 0;

        //! Sets the listener world orientation. By default { (0, 0, 0), (0, 0, 0) }.
        virtual void SetListenerOrientation(const ListenerOrientation& orientation) = 0;
        
        //! Returns the listener world position.
        virtual ListenerOrientation GetListenerOrientation() const = 0;

        /* ----- Audio data access ------ */

        /**
        \brief Determines the audio format of the specified stream.
        \param[in,out] stream Specifies the input stream where the audio format is to be determined from.
        \remarks This function will jump to the beginning of the input stream to read the magic number
        and then sets the reading position back to the previous position.
        \return Determined audio format or AudioFormats::Unknown if the audio format is not supported.
        */
        static AudioFormats DetermineAudioFormat(std::istream& stream);

        /**
        \brief Reads the audio data from the specified file and stores it in the output wave buffer.
        \param[in] filename Specifies the filename of the input file stream.
        \see ReadWaveBuffer(const AudioFormats, std::istream&, WaveBuffer&)
        */
        WaveBuffer ReadWaveBuffer(const std::string& filename);

        /**
        \brief Reads the audio data from the specified stream and stores it in the output wave buffer.
        \param[in,out] stream Specifies the input stream to read from. This stream must be opened in binary mode!
        \throws std::runtime_exception If something went wrong while reading.
        */
        WaveBuffer ReadWaveBuffer(std::istream& stream);

        /**
        \brief Opens a new audio stream form the specified file.
        \param[in] filename Specifies the filename of the input file stream.
        \see OpenAudioStream(std::istream&)
        */
        std::unique_ptr<AudioStream> OpenAudioStream(const std::string& filename);

        /**
        \brief Opens a new audio stream.
        \param[in] stream Specifies the input stream to read from. This stream must be opened in binary mode!
        \return New AudioStream object or null if 'format' is invalid.
        \remarks The input stream must be a unique pointer, so that the returned audio stream object can take care of the input stream to read from.
        \throws std::runtime_exception If something went wrong while opening the stream.
        */
        std::unique_ptr<AudioStream> OpenAudioStream(std::unique_ptr<std::istream>&& stream);

        /**
        \brief Writes the audio data to the specified stream.
        \param[in,out] stream Specifies the output stream to write to. This stream must be opened in binary mode!
        \param[out] waveBuffer Specifies the input wave buffer.
        \return True if the stream has been written successfully.
        \throws std::runtime_exception If something went wrong while writing.
        */
        bool WriteAudioBuffer(const AudioFormats format, std::ostream& stream, const WaveBuffer& waveBuffer);

        /* ----- Microphone ----- */

        std::unique_ptr<Microphone> QueryMicrophone();

    protected:

        AudioSystem() = default;

    private:

        struct ImmediateSoundDesc
        {
            std::string filename;
            float       volume;
            float       pitch;
        };

        void SoundMngrThreadProc();

        std::string                         name_;

        std::mutex                          soundMngrMutex_;
        std::thread                         soundMngrThread_;

        std::vector<ImmediateSoundDesc>     immediateSoundsQueue_;

};


} // /namespace Ac


#endif



// ================================================================================
