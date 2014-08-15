#ifndef ENGINE_VIDEO_TEXT_H
#define ENGINE_VIDEO_TEXT_H

#include <string>
#include <SDL/SDL_ttf.h>
#include "filetexture.h"
#include "color.h"
#include "font.h"

namespace engine
{
namespace video
{

class Text : public FileTexture
{
	public:
		Text(std::string string, Font* font, Color color);
		~Text();

		static void open()
		{
			TTF_Init();
		}

		static void close()
		{
			TTF_Quit();
		}
		
		inline const std::string& getString() const { return m_string; }
		inline Font* getFont() const { return m_font; }
		inline Color getColor() const { return m_color; }
		
	protected:
		std::string m_string;
		Font* m_font;
		Color m_color;
};

} // video
} // engine

#endif // ENGINE_VIDEO_TEXT_H


