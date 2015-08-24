#ifndef FLAT_VIDEO_VIDEO_H
#define FLAT_VIDEO_VIDEO_H

#include <memory>
#include <unordered_map>
#include "window.h"
#include "color.h"

namespace flat
{
namespace video
{
class FileTexture;

class Video
{
	public:
		Video();
		~Video();
		
		void endFrame();
		
		void clear();
		void setClearColor(const Color& color);
		
		std::shared_ptr<const FileTexture> getTexture(const std::string& fileName);

	public:
		Window* window;
		
	private:
		std::unordered_map<std::string, std::weak_ptr<const FileTexture>> m_loadedTextures;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIDEO_H


