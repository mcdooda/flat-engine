#ifndef FLAT_VIDEO_FONT_FONT_H
#define FLAT_VIDEO_FONT_FONT_H

#include <array>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include "misc/vector.h"

namespace flat
{
namespace video
{
namespace font
{

class Font
{
	public:
		struct CharInfoUv
		{
			float x;
			float y;
			CharInfoUv() : x(0.f), y(0.f) {}
			CharInfoUv(float x, float y) : x(x), y(y) {}
		};
		struct CharInfo
		{
			std::array<CharInfoUv, 6> uv;
			float minX;
			float maxX;
			float minY;
			float maxY;
			float advance;
			bool visible;
		};
		
	public:
		Font() = delete;
		Font(const Font&) = delete;
		Font(Font&&) = delete;
		Font(const std::string& font, int size);
		~Font();
		Font& operator=(const Font&) = delete;
		
		GLint getAtlasId() const { return m_atlasId; }
		const Vector2& getAtlasSize() const { return m_atlasSize; }

		inline float getAscent() const { return m_ascent; }
		inline float getDescent() const { return m_descent; }
		inline float getLineSkip() const { return m_lineSkip; }
		inline float getHeight() const { return m_height; }

		inline TTF_Font* getSdlFont() const { return m_font; }
		
		static void open();
		static void close();
		
		const CharInfo& getCharInfo(char c) const;
		bool isValidChar(char c) const;
		
	protected:
		static int getClosestPowerOf2(int value);

		CharInfo& getCharInfo(char c);
		
		enum { ATLAS_FIRST_CHAR = 32, ATLAS_LAST_CHAR = 126, ATLAS_NUM_CHARS = ATLAS_LAST_CHAR - ATLAS_FIRST_CHAR + 1 };
		
		std::array<CharInfo, ATLAS_NUM_CHARS> m_chars;
		TTF_Font* m_font;
		int m_fontSize;
		GLuint m_atlasId;
		Vector2 m_atlasSize;
		float m_ascent;
		float m_descent;
		float m_lineSkip;
		float m_height;
};

} // font
} // video
} // flat

#endif // FLAT_VIDEO_FONT_FONT_H


