#ifndef ENGINE_VIDEO_FILETEXTURE_H
#define ENGINE_VIDEO_FILETEXTURE_H

#include <string>
#include <SDL2/SDL.h>
#include "../geometry/vector2.h"
#include "color.h"
#include "texture.h"

namespace engine
{
namespace video
{

class FileTexture : public Texture
{
	public:
		FileTexture(std::string filename);

		inline const std::string& getPath() const { return m_path; }
		
		Color getPixel(const geometry::Vector2& pixelPosition);

	protected:
		FileTexture() {}
		
		void load();
		void free();
		
		SDL_Surface* m_surface;

		std::string m_path;
};

} // video
} // engine

#endif // ENGINE_VIDEO_FILETEXTURE_H


