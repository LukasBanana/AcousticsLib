/*
 * NullAudioSystem.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef AC_NULL_AUDIO_SYSTEM_H
#define AC_NULL_AUDIO_SYSTEM_H


#include <Ac/AudioSystem.h>
#include <thread>
#include <mutex>
#include <chrono>


namespace Ac
{


class NullSound;

class NullAudioSystem : public AudioSystem
{

    public:

        NullAudioSystem();
        ~NullAudioSystem();

        std::string GetVersion() const override;

        std::unique_ptr<Sound> CreateSound() override;

        void SetListenerPosition(const Gs::Vector3f& position) override;
        Gs::Vector3f GetListenerPosition() const override;

        void SetListenerVelocity(const Gs::Vector3f& velocity) override;
        Gs::Vector3f GetListenerVelocity() const override;

        void SetListenerOrientation(const ListenerOrientation& orientation) override;
        ListenerOrientation GetListenerOrientation() const override;

    protected:

        friend class NullSound;

        void RegisterSound(NullSound* sound);
        void UnregisterSound(NullSound* sound);

        inline const std::shared_ptr<bool>& GetSharedLiveFlag() const
        {
            return sharedLiveFlag_;
        }

    private:

        using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

        void SoundManagerThreadFunc();
        bool UpdateSoundState(NullSound* sound, double timeDelta);
        void ReleaseThread();

        Gs::Vector3f            listenerPosition_;
        Gs::Vector3f            listenerVelocity_;
        ListenerOrientation     listenerOrientation_;

        std::shared_ptr<bool>   sharedLiveFlag_;
        std::mutex              soundManagerMutex_;

        // synchronized {
        std::vector<NullSound*> playingSounds_;
        bool                    release_            = false;
        // }

        std::thread             soundManagerThread_;

};


} // /namespace Ac


#endif



// ================================================================================
