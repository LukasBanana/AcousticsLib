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

#include <string>
#include <memory>
#include <vector>


namespace Ac
{


//! Audio system interface.
class AC_EXPORT AudioSystem
{

    public:

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

        //! Loads the specified sound from file.
        virtual std::unique_ptr<Sound> LoadSound(const std::string& filename) = 0;

    protected:

        AudioSystem() = default;

    private:

        std::string name_;

};


} // /namespace Ac


#endif



// ================================================================================
