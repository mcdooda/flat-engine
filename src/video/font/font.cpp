#include "font.h"

namespace flat
{
namespace video
{
namespace font
{

Font::Font(const std::string& font, int size) :
	m_fontSize(size),
	m_atlasId(0)
{
	m_font = TTF_OpenFont(font.c_str(), size);
	
	int x = 0;
	for (char c = ATLAS_FIRST_CHAR; c <= ATLAS_LAST_CHAR; ++c)
	{
		CharInfo& ci = getCharInfo(c);
		TTF_GlyphMetrics(m_font, c, &ci.minX, &ci.maxX, &ci.minY, &ci.maxY, &ci.advance);
		x += ci.advance;
	}
	m_atlasSize.x = static_cast<float>(x);
	m_atlasSize.y = static_cast<float>(TTF_FontHeight(m_font));
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_atlasId);
	glBindTexture(GL_TEXTURE_2D, m_atlasId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(m_atlasSize.x), static_cast<GLsizei>(m_atlasSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	x = 0;
	for (char c = ATLAS_FIRST_CHAR; c <= ATLAS_LAST_CHAR; ++c)
	{
		CharInfo& ci = getCharInfo(c);
		char character[] = {c, '\0'};
		const SDL_Color sdlColor = {255, 255, 255};
		SDL_Surface* glyphSurface = TTF_RenderUTF8_Blended(m_font, character, sdlColor);
		if (glyphSurface)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyphSurface->w, glyphSurface->h, GL_RGBA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
			ci.visible = true;
		}
		else
		{
			ci.visible = false;
		}
		
		containers::FixedSizeArray<CharInfoUv, 6>& uv = ci.uv;
		
		float fx0 = static_cast<float>(x) / m_atlasSize.x;
		float fx1 = (static_cast<float>(x) + ci.advance) / m_atlasSize.x;
		
		uv[0].x = fx0;
		uv[0].y = 0.f;
		
		uv[1].x = fx1;
		uv[1].y = 0.f;
		
		uv[2].x = fx0;
		uv[2].y = 1.f;
		
		uv[3].x = fx1;
		uv[3].y = 1.f;
		
		uv[4].x = fx0;
		uv[4].y = 1.f;
		
		uv[5].x = fx1;
		uv[5].y = 0.f;
		
		x += ci.advance;
	}
}

Font::~Font()
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
}

void Font::open()
{
	TTF_Init();
}

void Font::close()
{
	TTF_Quit();
}

const Font::CharInfo& Font::getCharInfo(char c) const
{
	return m_chars[c - ATLAS_FIRST_CHAR];
}

Font::CharInfo& Font::getCharInfo(char c)
{
	return m_chars[c - ATLAS_FIRST_CHAR];
}

} // font
} // video
} // flat


