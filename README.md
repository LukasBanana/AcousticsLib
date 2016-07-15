AcousticsLib - Simple Cross Platform Audio Library
==================================================

License
-------

[3-Clause BSD License](https://github.com/LukasBanana/AcousticsLib/blob/master/LICENSE.txt)


Status
------

**Alpha**


Getting Started
---------------

```cpp
#include <Ac/AcLib.h>

int main()
{
	// If an audio system is not available, dynamically catch the exception an load another audio system
	// e.g. when "XAudio2" is not available because the respective DirectX version is missing on a Windows platform.
	for (auto module : std::vector<std::string> { "XAudio2", "OpenAL" })
	{
		try
		{
            // Load audio system from shared library
            auto audioSystem = Ac::AudioSystem::Load(module);
			
			std::cout << "Audio System: \" << audioSystem->GetName() << "\"" << std::endl;
			
			//to be continued ...
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return 0;
}
```


