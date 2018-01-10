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
#include "../Core/Streaming.h"

#include <Ac/AudioSystem.h>
#include <array>
#include <fstream>
#include <cstdint>
#include <functional>
#include <map>

//!!!TESTING!!!
#include <iostream>
#ifdef _WIN32
#include "../Platform/Win32/Win32Microphone.h"
#endif


namespace Ac
{


/* ----- Audio system ----- */

static std::map<AudioSystem*, std::unique_ptr<Module>> g_audioSystemModules;

AudioSystem::~AudioSystem()
{
    /* Detach immediate sound thread */
    if (soundMngrThread_.joinable())
        soundMngrThread_.detach();
}

std::vector<std::string> AudioSystem::FindModules()
{
    /* Iterate over all known modules and return those that are availale on the current platform */
    const std::array<std::string, 3> knownModules {{ "OpenAL", "XAudio2", "Null" }};
    
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

std::unique_ptr<AudioSystem> AudioSystem::Load(const std::string& moduleName)
{
    /* Load audio system module */
    auto moduleFilename = Module::GetModuleFilename(moduleName);
    auto module = Module::Load(moduleFilename);

    try
    {
        /* Allocate audio system */
        auto audioSystem = std::unique_ptr<AudioSystem>(LoadAudioSystem(*module, moduleFilename));
        audioSystem->name_ = LoadAudioSystemName(*module);

        /* Store new module globally */
        g_audioSystemModules[audioSystem.get()] = std::move(module);

        /* Return new audio system and unique pointer */
        return audioSystem;
    }
    catch (const std::exception&)
    {
        /* Keep module, otherwise the exception's vtable might be corrupted because it's part of the module */
        g_audioSystemModules[nullptr] = std::move(module);
        throw;
    }
}

std::unique_ptr<AudioSystem> AudioSystem::Load()
{
    /* Get all available audio system modules */
    auto modules = FindModules();
    if (modules.empty())
        throw std::runtime_error("no audio system module found");

    /* Try to load modules until a valid audio system has been found */
    for (const auto& moduleName : modules)
    {
        try
        {
            if (auto audioSystem = Load(moduleName))
                return std::move(audioSystem);
        }
        catch (const std::runtime_error&)
        {
            /* Ignore exception here, and try next module */
        }
    }

    throw std::runtime_error("failed to load any available audio system module");
}

void AudioSystem::Unload(std::unique_ptr<AudioSystem>&& audioSystem)
{
    auto it = g_audioSystemModules.find(audioSystem.get());
    if (it != g_audioSystemModules.end())
    {
        audioSystem.release();
        g_audioSystemModules.erase(it);
    }
}

/* ----- Sounds ----- */

std::unique_ptr<Sound> AudioSystem::CreateSound(const WaveBuffer& waveBuffer)
{
    auto sound = CreateSound();
    sound->AttachBuffer(waveBuffer);
    return sound;
}

static bool IsAudioStream(const AudioFormats format)
{
    switch (format)
    {
        case AudioFormats::OggVorbis:
        case AudioFormats::AmigaModule:
            return true;
        default:
            return false;
    }
}

std::unique_ptr<Sound> AudioSystem::LoadSound(const std::string& filename, const SoundFlags::BitMask flags)
{
    auto sound = CreateSound();

    /* Open binary input file stream */
    auto file = std::unique_ptr<std::ifstream>(new std::ifstream(filename, std::ios_base::binary));
    if (file->good())
    {
        /* Determine audio file format */
        auto format = Ac::DetermineAudioFormat(*file);

        if (IsAudioStream(format))
        {
            /* Load sound as audio stream */
            auto audioStream = OpenAudioStream(std::move(file));
            sound->SetStreamSource(std::move(audioStream));
        }
        else
        {
            /* Load sound as wave buffer */
            auto waveBuffer = ReadWaveBuffer(*file);

            if ((flags & SoundFlags::Enable3D) != 0)
                waveBuffer.SetChannels(1);

            sound->AttachBuffer(waveBuffer);
        }
    }
    else if ((flags & SoundFlags::AlwaysCreateSound) == 0)
        return nullptr;

    /* Appply further flags */
    if ((flags & SoundFlags::Enable3D) != 0)
        sound->Enable3D();

    return sound;
}

void AudioSystem::Play(const std::string& filename, float volume, float pitch)
{
    /* Add descriptor to immediate sound queue */
    std::lock_guard<std::mutex> lock(soundMngrMutex_);
    immediateSoundsQueue_.push_back({ filename, volume, pitch });

    /* Ensure thread is running */
    if (!soundMngrThread_.joinable())
        soundMngrThread_ = std::thread(std::bind(&AudioSystem::SoundMngrThreadProc, this));
}

void AudioSystem::Streaming(Sound& sound, WaveBuffer& waveBuffer)
{
    Ac::Streaming(sound, waveBuffer);
}

void AudioSystem::Streaming(Sound& sound)
{
    Ac::Streaming(sound);
}

/* ----- Audio data access ------ */

AudioFormats AudioSystem::DetermineAudioFormat(std::istream& stream)
{
    return Ac::DetermineAudioFormat(stream);
}

WaveBuffer AudioSystem::ReadWaveBuffer(const std::string& filename)
{
    /* Open file stream in binary mode */
    std::ifstream file(filename, std::ios_base::binary);
    return (file.good() ? ReadWaveBuffer(file) : WaveBuffer());
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
            
        default:
            break;
    }
    return nullptr;
}

WaveBuffer AudioSystem::ReadWaveBuffer(std::istream& stream)
{
    WaveBuffer waveBuffer;
    
    auto reader = QueryReader(Ac::DetermineAudioFormat(stream));
    if (reader)
        reader->ReadWaveBuffer(stream, waveBuffer);

    return waveBuffer;
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
            #ifdef AC_PLUGIN_OGGVORBIS
            case AudioFormats::OggVorbis:
                return std::unique_ptr<AudioStream>(new OGGStream(std::move(stream)));
            #endif

            case AudioFormats::AmigaModule:
                return std::unique_ptr<AudioStream>(new MODStream(std::move(stream)));
                
            default:
                break;
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
        default:
            return nullptr;
    }
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

void AudioSystem::SoundMngrThreadProc()
{
    /* Use local buffer for audio streaming to avoid race conditions */
    WaveBuffer streamingBuffer;

    std::list<std::unique_ptr<Sound>> sounds;

    while (true)
    {
        /* Remove unused immediate sounds from the list */
        for (auto it = sounds.begin(); it != sounds.end();)
        {
            auto& snd = **it;
            if (snd.IsPlaying())
            {
                /* Process streaming (if the sound has an audio stream) */
                Ac::Streaming(snd, streamingBuffer);
                ++it;
            }
            else
                it = sounds.erase(it);
        }

        {
            std::lock_guard<std::mutex> lock(soundMngrMutex_);

            /* Check if new sounds must be loaded */
            for (const auto& desc : immediateSoundsQueue_)
            {
                auto sound = LoadSound(desc.filename);
                if (sound)
                {
                    sound->SetVolume(desc.volume);
                    sound->SetPitch(desc.pitch);
                    sound->Play();

                    sounds.push_back(std::move(sound));
                }
            }

            immediateSoundsQueue_.clear();

            /* Check if thread can be terminated */
            if (sounds.empty())
            {
                if (soundMngrThread_.joinable())
                    soundMngrThread_.detach();
                return;
            }
        }

        /* Wait a moment to give other processes time to run */
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


} // /namespace Ac



// ================================================================================
