#ifndef FLAT_VIDEO_FONT_H
#define FLAT_VIDEO_FONT_H

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>
#include "../../geometry/vector2.h"
#include "../../containers/fixedsizearray.h"

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
		};
		struct CharInfo
		{
			containers::FixedSizeArray<CharInfoUv, 6> uv;
			int minX;
			int maxX;
			int minY;
			int maxY;
			int advance;
			bool visible;
		};
		
	public:
		Font(const std::string& font, int size);
		~Font();
		
		GLint getAtlasId() const { return m_atlasId; }
		const geometry::Vector2& getAtlasSize() const { return m_atlasSize; }
		
		static void open();
		static void close();
		
		const CharInfo& getCharInfo(char c) const;
		
	protected:
		CharInfo& getCharInfo(char c);
		
		enum { ATLAS_FIRST_CHAR = 32, ATLAS_LAST_CHAR = 126, ATLAS_NUM_CHARS = ATLAS_LAST_CHAR - ATLAS_FIRST_CHAR + 1 };
		
		containers::FixedSizeArray<CharInfo, ATLAS_NUM_CHARS> m_chars;
		TTF_Font* m_font;
		int m_fontSize;
		GLuint m_atlasId;
		geometry::Vector2 m_atlasSize;
};

} // font
} // video
} // flat

#endif // FLAT_VIDEO_FONT_H


