#ifndef ENGINE_VIDEO_FONT_H
#define ENGINE_VIDEO_FONT_H

#include <string>
#include <SDL/SDL_ttf.h>

namespace engine
{
namespace video
{

class Font
{
	public:
		Font(std::string font);
		~Font();
		
	protected:
		friend class Text;
		
		TTF_Font* m_font;
};

} // video
} // engine

#endif // ENGINE_VIDEO_FONT_H


