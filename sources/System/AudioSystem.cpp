/*
 * AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Ac/AudioSystem.h>
#include "../Platform/Module.h"
#include <array>


namespace Ac
{


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


} // /namespace Ac



// ================================================================================
