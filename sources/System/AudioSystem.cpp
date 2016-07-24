/*
 * AudioSystem.cpp
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Platform/Module.h"
#include "../FileHandler/WAVReader.h"
#include "../FileHandler/WAVWriter.h"
#include "../FileHandler/AIFFReader.h"
#include "../FileHandler/OGGStream.h"
#include "../FileHandler/MODStream.h"
#include "../FileHandler/FileType.h"

#include <Ac/AudioSystem.h>
#include <array>
#include <fstream>
#include <cstdint>

//!!!TESTING!!!
#ifdef _WIN32
#include "../Platform/Win32/Win32Microphone.h"
#endif


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

std::unique_ptr<Sound> AudioSystem::CreateSound(const WaveBuffer& waveBuffer)
{
    auto sound = CreateSound();
    sound->AttachBuffer(waveBuffer);
    return sound;
}

std::unique_ptr<Sound> AudioSystem::LoadSound(const std::string& filename, const SoundFlags::BitMask flags)
{
    auto sound = CreateSound();

    if (IsFileAudioStream(filename))
    {
        auto audioStream = OpenAudioStream(filename);
        sound->SetStreamSource(std::move(audioStream));
    }
    else
    {
        auto waveBuffer = ReadAudioBuffer(filename);
        if (waveBuffer)
        {
            if ((flags & SoundFlags::Enable3D) != 0)
                waveBuffer->SetChannels(1);
            sound->AttachBuffer(*waveBuffer);
        }
        else if ((flags & SoundFlags::AlwaysCreateSound) == 0)
            return nullptr;
    }

    if ((flags & SoundFlags::Enable3D) != 0)
        sound->Enable3D();

    return sound;
}

void AudioSystem::Play(const std::string& filename, float volume, std::size_t repetitions, const std::function<bool(Sound&)> waitCallback)
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

AudioFormats AudioSystem::DetermineAudioFormat(std::istream& stream)
{
    return Ac::DetermineAudioFormat(stream);
}

std::unique_ptr<WaveBuffer> AudioSystem::ReadAudioBuffer(const std::string& filename)
{
    /* Open file stream in binary mode */
    std::ifstream file(filename, std::ios_base::binary);

    if (file.good())
    {
        /* Read audio buffer from stream */
        auto waveBuffer = std::unique_ptr<WaveBuffer>(new WaveBuffer());
        ReadAudioBuffer(file, *waveBuffer);
        return waveBuffer;
    }

    return nullptr;
}

static std::unique_ptr<AudioReader> QueryReader(const AudioFormats format)
{
    switch (format)
    {
        case AudioFormats::WAVE:
            return std::unique_ptr<AudioReader>(new WAVReader());

        case AudioFormats::AIFF:
        case AudioFormats::AIFC:
            return std::unique_ptr<AudioReader>(new AIFFReader());

        case AudioFormats::OggVorbis:
        case AudioFormats::AmigaModule:
            throw std::runtime_error("can not read entire wave buffer from audio stream");
            break;
    }
    return nullptr;
}

void AudioSystem::ReadAudioBuffer(std::istream& stream, WaveBuffer& waveBuffer)
{
    auto reader = QueryReader(Ac::DetermineAudioFormat(stream));
    if (reader)
        reader->ReadWaveBuffer(stream, waveBuffer);
}

std::unique_ptr<AudioStream> AudioSystem::OpenAudioStream(const std::string& filename)
{
    /* Open file stream in binary mode */
    auto file = std::unique_ptr<std::ifstream>(new std::ifstream(filename, std::ios_base::binary));
    return (file->good() ? OpenAudioStream(std::move(file)) : nullptr);
}

std::unique_ptr<AudioStream> AudioSystem::OpenAudioStream(std::unique_ptr<std::istream>&& stream)
{
    if (stream)
    {
        switch (Ac::DetermineAudioFormat(*stream))
        {
            case AudioFormats::OggVorbis:
                #ifdef AC_PLUGIN_OGGVORBIS
                return std::unique_ptr<AudioStream>(new OGGStream(std::move(stream)));
                #else
                break;
                #endif

            case AudioFormats::AmigaModule:
                return std::unique_ptr<AudioStream>(new MODStream(std::move(stream)));
        }
    }
    return nullptr;
}

static std::unique_ptr<AudioWriter> QueryWriter(const AudioFormats format)
{
    switch (format)
    {
        case AudioFormats::WAVE:
            return std::unique_ptr<AudioWriter>(new WAVWriter());
    }
    return nullptr;
}

bool AudioSystem::WriteAudioBuffer(const AudioFormats format, std::ostream& stream, const WaveBuffer& waveBuffer)
{
    auto writer = QueryWriter(format);
    if (writer)
    {
        writer->WriteWaveBuffer(stream, waveBuffer);
        return true;
    }
    return false;
}

/* ----- Microphone ----- */

std::unique_ptr<Microphone> AudioSystem::QueryMicrophone()
{
    //!!!TESTING!!!
    #ifdef _WIN32
    return std::unique_ptr<Microphone>(new Win32Microphone());
    #endif
    return nullptr; //todo...
}


/*
 * ======= Private: =======
 */

bool AudioSystem::IsFileAudioStream(const std::string& filename) const
{
    //TODO -> perform magic number analysis of file!!!
    return (filename.size() >= 4 ? filename.substr(filename.size() - 4) == ".ogg" : false);
}


} // /namespace Ac



// ================================================================================
