/*
 * AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Platform/Module.h"
#include "../FileHandler/WAVReader.h"
#include "../FileHandler/OGGStream.h"
#include "../FileHandler/WAVWriter.h"

#include <Ac/AudioSystem.h>
#include <array>


namespace Ac
{


/* ----- Audio system ----- */

static std::weak_ptr<AudioSystem>   g_audioSystemRef;
static std::unique_ptr<Module>      g_audioSystemModule;

AudioSystem::~AudioSystem()
{
}

std::vector<std::string> AudioSystem::FindModules()
{
    /* Iterate over all known modules and return those that are availale on the current platform */
    const std::array<std::string, 2> knownModules {{ "OpenAL", "XAudio2" }};
    
    std::vector<std::string> modules;
    
    for (const auto& m : knownModules)
    {
        if (Module::IsAvailable(Module::GetModuleFilename(m)))
            modules.push_back(m);
    }
    
    return modules;
}
    
static AudioSystem* LoadAudioSystem(Module& module, const std::string& moduleFilename)
{
    /* Load "Ac_AudioSystem_Alloc" procedure */
    AC_PROC_INTERFACE(void*, PFN_AUDIOSYSTEM_ALLOC, (void));

    auto AudioSystem_Alloc = reinterpret_cast<PFN_AUDIOSYSTEM_ALLOC>(module.LoadProcedure("Ac_AudioSystem_Alloc"));
    if (!AudioSystem_Alloc)
        throw std::runtime_error("failed to load \"Ac_AudioSystem_Alloc\" procedure from module \"" + moduleFilename + "\"");

    return reinterpret_cast<AudioSystem*>(AudioSystem_Alloc());
}

static std::string LoadAudioSystemName(Module& module)
{
    /* Load "Ac_AudioSystem_Name" procedure and store its value in the name field */
    AC_PROC_INTERFACE(const char*, PFN_AUDIOSYSTEM_NAME, (void));

    auto AudioSystem_Name = reinterpret_cast<PFN_AUDIOSYSTEM_NAME>(module.LoadProcedure("Ac_AudioSystem_Name"));
    if (AudioSystem_Name)
        return std::string(AudioSystem_Name());

    return "";
}

std::shared_ptr<AudioSystem> AudioSystem::Load(const std::string& moduleName)
{
    /* Check if previous module can be safely released (i.e. the previous audio system has been deleted) */
    if (!g_audioSystemRef.expired())
        throw std::runtime_error("failed to load audio system (only a single instance can be loaded at a time)");

    /* Load audio system module */
    auto moduleFilename = Module::GetModuleFilename(moduleName);
    auto module = Module::Load(moduleFilename);
    auto audioSystem = std::shared_ptr<AudioSystem>(LoadAudioSystem(*module, moduleFilename));
    audioSystem->name_ = LoadAudioSystemName(*module);

    /* Store new module globally */
    g_audioSystemModule = std::move(module);
    g_audioSystemRef = audioSystem;

    /* Return new audio system and unique pointer */
    return audioSystem;
}

std::shared_ptr<AudioSystem> AudioSystem::Load()
{
    auto modules = FindModules();
    return (modules.empty() ? nullptr : Load(modules.front()));
}

/* ----- Sounds ----- */

void AudioSystem::PlaySound(const std::string& filename, float volume, std::size_t repetitions, const std::function<bool(Sound&)> waitCallback)
{
    /* Load and play sounds */
    auto sound = LoadSound(filename);
    
    if (sound)
    {
        sound->SetVolume(volume);
        sound->Play();
    
        /* Wait or add sound to immediate sound list */
        if (waitCallback)
        {
            while (sound->IsPlaying())
            {
                if (!waitCallback(*sound))
                {
                    immediateSounds_.push_back(std::move(sound));
                    break;
                }
            }
        }
        else
            immediateSounds_.push_back(std::move(sound));
    }
}

/* ----- Audio data access ------ */

void AudioSystem::ReadAudioBuffer(const AudioFormats format, std::istream& stream, WaveBuffer& waveBuffer)
{
    switch (format)
    {
        case AudioFormats::WAV:
            WAVReader reader;
            reader.ReadWaveBuffer(stream, waveBuffer);
            break;
    }
}

std::unique_ptr<AudioStream> AudioSystem::OpenAudioStream(const AudioStreamFormats format, std::istream& stream)
{
    switch (format)
    {
        case AudioStreamFormats::OGG:
            #ifdef AC_PLUGIN_OGGVORBIS
            return std::unique_ptr<AudioStream>(new OGGStream(stream));
            #endif
            break;
    }
    return nullptr;
}

void AudioSystem::WriteAudioBuffer(const AudioFormats format, std::ostream& stream, const WaveBuffer& waveBuffer)
{
    switch (format)
    {
        case AudioFormats::WAV:
            WAVWriter writer;
            writer.WriteWaveBuffer(stream, waveBuffer);
            break;
    }
}


} // /namespace Ac



// ================================================================================
