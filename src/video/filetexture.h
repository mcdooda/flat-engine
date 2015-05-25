#ifndef FLAT_VIDEO_FILETEXTURE_H
#define FLAT_VIDEO_FILETEXTURE_H

#include <string>
#include <SDL2/SDL.h>
#include "../geometry/vector2.h"
#include "color.h"
#include "texture.h"

namespace flat
{
namespace video
{

class FileTexture : public Texture
{
	public:
		FileTexture(const std::string& filename);
		~FileTexture() override;

		inline const std::string& getPath() const { return m_path; }
		
		Color getPixel(const geometry::Vector2& pixelPosition);

	protected:
		FileTexture();
		
		void load();
		void free();
		
		SDL_Surface* m_surface;

		std::string m_path;
};

} // video
} // flat

#endif // FLAT_VIDEO_FILETEXTURE_H


