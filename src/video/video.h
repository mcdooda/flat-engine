#ifndef FLAT_VIDEO_VIDEO_H
#define FLAT_VIDEO_VIDEO_H

#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "video/window.h"
#include "video/color.h"
#include "video/filetexture.h"
#include "video/font/font.h"

#include "resource/weakresourcemanager.h"

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

		inline std::shared_ptr<const FileTexture> getTexture(const std::filesystem::path& fileName) const { return m_textureManager.getResource(fileName.string()); }
		inline std::shared_ptr<const font::Font> getFont(const std::filesystem::path& fileName, int size) const { return m_fontManager.getResource(fileName.string(), size); }

	public:
		Window* window;

	private:
		resource::WeakResourceManager<FileTexture, std::string> m_textureManager;
		resource::WeakResourceManager<font::Font, std::string, int> m_fontManager;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIDEO_H


