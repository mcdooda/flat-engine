#include "font.h"

namespace engine
{
namespace video
{

Font::Font(std::string font)
{
	int atPos = font.find_first_of('@');
	const char* file = font.substr(0, atPos).c_str();
	int size = atoi(font.substr(atPos + 1).c_str());
	m_font = TTF_OpenFont(file, size);
}

Font::~Font()
{
	TTF_CloseFont(m_font);
}

} // video
} // engine


