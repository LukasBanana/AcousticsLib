/*
 * AudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_AUDIO_SYSTEM_H__
#define __AC_AUDIO_SYSTEM_H__


#include "Export.h"
#include "Sound.h"
#include "Sound3D.h"
#include "AudioStream.h"
#include "AudioFormats.h"
#include "Microphone.h"

#include <Gauss/Vector3.h>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <functional>


namespace Ac
{


//! Structure for the 3D listener orientation with at- and up- vectors.
struct AC_EXPORT ListenerOrientation
{
    Gs::Vector3f atVector;
    Gs::Vector3f upVector;
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

        //! Creates an empty 3D sound which can later be filled with a wave buffer.
        virtual std::unique_ptr<Sound3D> CreateSound3D() = 0;
    
        //! Creates a sound initialized with specified wave buffer.
        std::unique_ptr<Sound> CreateSound(const WaveBuffer& waveBuffer);

        //! Creates a 3D sound initialized with specified wave buffer.
        std::unique_ptr<Sound3D> CreateSound3D(const WaveBuffer& waveBuffer);

        /**
        \brief Loads the specified sound from file.
        \param[in] alwaysCreateSound Specifies whether to always create a sound or to
        return a null pointer if the file could not be found. By default false.
        */
        std::unique_ptr<Sound> LoadSound(const std::string& filename, bool alwaysCreateSound = false);

        /**
        \brief Loads the specified sound from file and returns it as a 3D sound.
        \see LoadSound
        */
        std::unique_ptr<Sound3D> LoadSound3D(const std::string& filename, bool alwaysCreateSound = false);

        /**
        \brief Play specified sound file.
        \param[in] filename Specifies the sound file to play.
        \param[in] volume Specifies the volume. By default 1.
        \param[in] repetitions Specifies the repetitions. By default 0.
        \param[in] waitCallback Specifies whether to wait until the sound has been played to the end.
        The callback can be used to cancel the waiting process. By default null.
        \remarks This is a 'very high level' function and is commonly used for tests only, to reduce the code to a minimum.
        */
        void PlaySound(
            const std::string& filename,
            float volume = 1.0f,
            std::size_t repetitions = 0,
            const std::function<bool(Sound&)> waitCallback = nullptr
        );

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
        \brief Reads the audio data from the specified file and stores it in the output wave buffer.
        \param[in] filename Specifies the filename of the input file stream.
        \see ReadAudioBuffer(const AudioFormats, std::istream&, WaveBuffer&)
        */
        std::unique_ptr<WaveBuffer> ReadAudioBuffer(const std::string& filename);

        /**
        \brief Reads the audio data from the specified stream and stores it in the output wave buffer.
        \param[in] format Specifies the file audio format.
        \param[in,out] stream Specifies the input stream to read from. This stream must be opened in binary mode!
        \param[out] waveBuffer Specifies the output wave buffer.
        \throws std::runtime_exception If something went wrong while reading.
        */
        void ReadAudioBuffer(const AudioFormats format, std::istream& stream, WaveBuffer& waveBuffer);

        /**
        \brief Opens a new audio stream form the specified file.
        \param[in] filename Specifies the filename of the input file stream.
        \see OpenAudioStream(const AudioStreamFormats, std::istream&)
        */
        std::unique_ptr<AudioStream> OpenAudioStream(const std::string& filename);

        /**
        \brief Opens a new audio stream.
        \param[in] stream Specifies the input stream to read from. This stream must be opened in binary mode!
        \return New AudioStream object or null if 'format' is invalid.
        \remarks The input stream must be a unique pointer, so that the returned audio stream object can take care of the input stream to read from.
        \throws std::runtime_exception If something went wrong while opening the stream.
        */
        std::unique_ptr<AudioStream> OpenAudioStream(const AudioStreamFormats format, std::unique_ptr<std::istream>&& stream);

        /**
        \brief Writes the audio data to the specified stream.
        \param[in,out] stream Specifies the output stream to write to. This stream must be opened in binary mode!
        \param[out] waveBuffer Specifies the input wave buffer.
        \throws std::runtime_exception If something went wrong while writing.
        */
        void WriteAudioBuffer(const AudioFormats format, std::ostream& stream, const WaveBuffer& waveBuffer);

        /* ----- Microphone ----- */

        std::unique_ptr<Microphone> QueryMicrophone();

    protected:

        AudioSystem() = default;

    private:

        bool LoadSoundFromFile(Sound& sound, const std::string& filename, bool alwaysCreateSound);

        std::string name_;

        std::list<std::unique_ptr<Sound>> immediateSounds_;

};


} // /namespace Ac


#endif



// ================================================================================
