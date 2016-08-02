#ifndef FLAT_VIDEO_VIDEO_H
#define FLAT_VIDEO_VIDEO_H

#include <memory>
#include <unordered_map>
#include <string>
#include "window.h"
#include "color.h"
#include "filetexture.h"
#include "font/font.h"
#include "../resource/resourcemanager.h"

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
		
		inline std::shared_ptr<const FileTexture> getTexture(const std::string& fileName) const { return m_textureManager.getResource(fileName); }
		inline std::shared_ptr<const font::Font> getFont(const std::string& fileName, int size) const { return m_fontManager.getResource(fileName, size); }

	public:
		Window* window;
		
	private:
		resource::ResourceManager<FileTexture, std::string> m_textureManager;
		resource::ResourceManager<font::Font, std::string, int> m_fontManager;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIDEO_H


