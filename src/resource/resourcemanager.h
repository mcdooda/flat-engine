#ifndef ENGINE_RESOURCE_RESOURCEMANAGER_H
#define ENGINE_RESOURCE_RESOURCEMANAGER_H

#include <map>
#include "video/texture.h"
#include "video/font.h"
#include "audio/sample.h"
#include "audio/music.h"

namespace engine
{
namespace resource
{

template <class T> class ResourceManager
{
	public:
		T* get(std::string name);

	private:
		T* load(std::string name);

		std::map<std::string, T*> m_resources;
};

class TextureManager : public ResourceManager<video::Texture> {};
class FontManager    : public ResourceManager<video::Font>    {};
class SampleManager  : public ResourceManager<audio::Sample>  {};
class MusicManager   : public ResourceManager<audio::Music>   {};

} // resource
} // engine

#include "resourcemanager.inl"

#endif // ENGINE_RESOURCE_RESOURCEMANAGER_H


