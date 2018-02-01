#ifndef FLAT_VIDEO_FILETEXTURE_H
#define FLAT_VIDEO_FILETEXTURE_H

#include <string>
#include <SDL2/SDL.h>
#include "../misc/vector.h"
#include "color.h"
#include "texture.h"

namespace flat
{
namespace video
{

class FileTexture : public Texture
{
	public:
		FileTexture(const std::string& fileName);
		~FileTexture() override;

		inline const std::string& getFileName() const { return m_fileName; }
		
		void getPixel(const Vector2& pixelPosition, Color& color) const;

	protected:
		FileTexture();
		
		void load();
		void free();

		void createPlaceholderTexture();

	protected:
		
		SDL_Surface* m_surface;

		std::string m_fileName;
};

} // video
} // flat

#endif // FLAT_VIDEO_FILETEXTURE_H


