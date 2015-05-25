#include <iostream>
#include "text.h"

namespace flat
{
namespace video
{

Text::Text(const std::string& string, Font* font, const Color& color) :
	m_string(string),
	m_font(font),
	m_color(color)
{
	SDL_Color sdlColor = {
		(unsigned char)(color.getB() * 255),
		(unsigned char)(color.getG() * 255),
		(unsigned char)(color.getR() * 255)
	};
	m_surface = TTF_RenderUTF8_Blended(font->m_font, string.c_str(), sdlColor);
	
	if (m_surface != nullptr)
		load();
		
	else
		std::cerr << "Warning: error in TTF_RenderText_Blended(" << string.c_str() << ") : " << TTF_GetError() << std::endl;
}

Text::~Text()
{
	
}

} // video
} // flat


