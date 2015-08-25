#ifndef FLAT_VIDEO_VIDEO_H
#define FLAT_VIDEO_VIDEO_H

#include <memory>
#include <unordered_map>
#include <string>
#include "window.h"
#include "color.h"
#include "filetexture.h"
#include "../resource/resourcemanager.h"

namespace flat
{
namespace video
{
class FileTexture;

class Video : public resource::ResourceManager<FileTexture, std::string>
{
	public:
		Video();
		~Video();
		
		void endFrame();
		
		void clear();
		void setClearColor(const Color& color);
		
		inline std::shared_ptr<const FileTexture> getTexture(const std::string& fileName) { return getResource(fileName); }

	public:
		Window* window;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIDEO_H


