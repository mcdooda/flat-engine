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
} // flat

#include "resourcemanager.inl"

#endif // FLAT_RESOURCE_RESOURCEMANAGER_H


