#ifndef FLAT_VIDEO_FONT_H
#define FLAT_VIDEO_FONT_H

#include <string>
#include <SDL/SDL_ttf.h>

namespace flat
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
} // flat

#endif // FLAT_VIDEO_FONT_H


