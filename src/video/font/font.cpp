#include <iostream>
#include <cctype>
#include "video/font/font.h"
#include "console/console.h"

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
	FLAT_ASSERT(m_font != nullptr);

	m_ascent = static_cast<float>(TTF_FontAscent(m_font));
	m_descent = static_cast<float>(TTF_FontDescent(m_font));
	m_lineSkip = static_cast<float>(TTF_FontLineSkip(m_font));
	m_height = static_cast<float>(TTF_FontHeight(m_font));

	constexpr float characterSpacing = 2.f;
	
	float x = 0;
	for (char c = ATLAS_FIRST_CHAR; c <= ATLAS_LAST_CHAR; ++c)
	{
		CharInfo& ci = getCharInfo(c);
		ci.visible = false; // will be overriden if successfully added to the atlas
		int minX, maxX, minY, maxY, advance;
		const int result = TTF_GlyphMetrics(m_font, c, &minX, &maxX, &minY, &maxY, &advance);
		FLAT_ASSERT(result == 0);
		ci.minX = static_cast<float>(minX);
		ci.maxX = static_cast<float>(maxX);
		ci.minY = static_cast<float>(minY);
		ci.maxY = static_cast<float>(maxY);
		ci.advance = static_cast<float>(advance);
		x += ci.advance + characterSpacing;
	}

	const float atlasWidth = static_cast<float>(getClosestPowerOf2(static_cast<int>(x)));
	const float atlasHeight = static_cast<float>(getClosestPowerOf2(static_cast<int>(m_height) + 3)); // add some margin as TTF_RenderUTF8_Blended can render characters that are higher than this

	m_atlasSize = flat::Vector2(atlasWidth, atlasHeight);
	
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

		if (isValidChar(c))
		{
			const char character[] = { c, '\0' };
			static const SDL_Color sdlColor = { 255, 255, 255 };
			SDL_Surface* glyphSurface = TTF_RenderUTF8_Blended(m_font, character, sdlColor);

			bool couldRenderCharacter = false;

			if (glyphSurface != nullptr)
			{
				if (atlasHeight >= glyphSurface->h)
				{
					glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(x), 0, glyphSurface->w, glyphSurface->h, GL_RGBA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
					GLenum errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						ci.visible = true;
						couldRenderCharacter = true;
					}
					else
					{
						FLAT_CONSOLE_COLOR(LIGHT_RED);
						std::cerr << "Could not render character '" << c << "' in font " << font << " (glTexSubImage2D failed with code 0x" << std::hex << errorCode << std::dec << ")" << std::endl;
					}
				}
				else
				{
					FLAT_CONSOLE_COLOR(LIGHT_RED);
					std::cerr << "Could not render character '" << c << "' in font " << font << " (TTF_RenderUTF8_Blended rendered glyph of height " << glyphSurface->h << ", max is " << m_height << ")" << std::endl;
				}

				SDL_FreeSurface(glyphSurface);
			}
			else
			{
				FLAT_CONSOLE_COLOR(LIGHT_RED);
				std::cerr << "Could not render character '" << c << "' in font " << font << " (TTF_RenderUTF8_Blended failed)" << std::endl;
			}

			std::array<CharInfoUv, 6>& uv = ci.uv;

			const float characterOffsetX = ci.minX < 0.f ? -ci.minX / atlasWidth : 0.f;

			const float fx0 = (x + ci.minX) / atlasWidth + characterOffsetX;
			const float fx1 = (x + ci.maxX) / atlasWidth + characterOffsetX;

			const float fy0 = (m_ascent - ci.maxY) / atlasHeight;
			const float fy1 = (m_ascent - ci.minY) / atlasHeight;

			uv[0].x = fx0;
			uv[0].y = fy0;

			uv[1].x = fx1;
			uv[1].y = fy0;

			uv[2].x = fx0;
			uv[2].y = fy1;

			uv[3].x = fx1;
			uv[3].y = fy1;

			uv[4].x = fx0;
			uv[4].y = fy1;

			uv[5].x = fx1;
			uv[5].y = fy0;
		}

		x += ci.advance + characterSpacing;
	}
}

Font::~Font()
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
	glDeleteTextures(1, &m_atlasId);
}

void Font::open()
{
	TTF_Init();
}

void Font::close()
{
	TTF_Quit();
}

int Font::getClosestPowerOf2(int value)
{
	return static_cast<int>(std::pow(2, std::ceil(std::log(value) / std::log(2))));
}

const Font::CharInfo& Font::getCharInfo(char c) const
{
	return m_chars[c - ATLAS_FIRST_CHAR];
}

bool Font::isValidChar(char c) const
{
	return ATLAS_FIRST_CHAR <= c && c <= ATLAS_LAST_CHAR && std::isprint(c) && !std::isspace(c);
}

Font::CharInfo& Font::getCharInfo(char c)
{
	return m_chars[c - ATLAS_FIRST_CHAR];
}

} // font
} // video
} // flat


