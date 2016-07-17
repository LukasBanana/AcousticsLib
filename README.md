AcousticsLib - Simple Cross Platform Audio Library
==================================================

License
-------

[3-Clause BSD License](https://github.com/LukasBanana/AcousticsLib/blob/master/LICENSE.txt)


Status
------

**Alpha**


Platform Support
----------------

* **Windows Vista/ 7/ 8/ 10**
* **Mac OS X**
* **Linux**


Audio Engines
-------------

* **OpenAL** *(in progress)*
* **XAudio2** *(planed, but not started yet)*
* more coming ...


Getting Started
---------------

```cpp
#include <Ac/AcLib.h>
#include <iostream>

int main()
{
	// If an audio system is not available, dynamically catch the
	// exception an load another audio system, e.g. when "XAudio2"
	// is not available due to a missing DirectX version on a Windows platform.
	for (auto module : Ac::AudioSystem::FindModules())
	{
		try
		{
            // Load audio system from shared library
            auto audioSystem = Ac::AudioSystem::Load(module);
			
			std::cout
				<< "Audio System: \" << audioSystem->GetName() << "\""
				<< ", Version: " << audioSystem->GetVersion() << std::endl;
			
			auto sound = audioSystem->LoadSound("mySound.wav");
			if (sound)
			{
				sound->SetVolume(0.5f); // set volume to 50%
				sound->SetPitch(1.5f);  // play with 1.5x frequency (or speed)
				sound->Play();          // play sound
				
				while (sound->IsPlaying())
				{
					// Wait while sound is playing and show current seek position
					std::cout << "Playing: " << sound->GetSeek() << std::end;
				}
			}
			
			// Break out from loop, we only need one single audio system
			break;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return 0;
}
```


