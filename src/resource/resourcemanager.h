#ifndef FLAT_RESOURCE_RESOURCEMANAGER_H
#define FLAT_RESOURCE_RESOURCEMANAGER_H

#include <map>
#include "video/texture.h"
#include "video/font.h"
#include "audio/sample.h"
#include "audio/music.h"

namespace flat
{
namespace resource
{

template <class T>
class ResourceManager
{
	public:
		T* get(const std::string& name)
		{
			T* resource;
			typename std::map<std::string, T*>::iterator it = m_resources.find(name);

			if (it != m_resources.end())
				resource = it->second;
	
			else
				resource = load(name);

			return resource;
		}

	private:
		T* load(const std::string& name)
		{
			T* const resource = new T(name);
			m_resources[name] = resource;
			return resource;
		}

	private:
		std::map<std::string, T*> m_resources;
};

typedef ResourceManager<video::Texture> TextureManager;
typedef ResourceManager<video::Font>    FontManager;
typedef ResourceManager<audio::Sample>  SampleManager;
typedef ResourceManager<audio::Music>   MusicManager;

} // resource
} // flat

#endif // FLAT_RESOURCE_RESOURCEMANAGER_H


