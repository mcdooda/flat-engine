#ifndef FLAT_VIDEO_FONT_STRING_H
#define FLAT_VIDEO_FONT_STRING_H

#include <memory>
#include <string>
#include <vector>
#include "font.h"

namespace flat
{
namespace video
{
namespace font
{

class String
{
	struct CharacterVertex
	{
		float x;
		float y;
		CharacterVertex(float x, float y) : x(x), y(y) {}
	};
	
	public:
		String(std::shared_ptr<const Font> font);
		~String();
		
		inline void setWrapLength(int wrapLength) { m_wrapLength = wrapLength; }
		inline void setNoWrap() { setWrapLength(0); }
		
		virtual void setText(const std::string& text);
		inline const std::string& getText() const { return m_text; }
		
		inline const std::vector<CharacterVertex>& getVertices() const { return m_vertices; }
		inline const std::vector<Font::CharInfoUv>& getUv() const { return m_uv; }
		
		inline std::shared_ptr<const Font> getFont() const { return m_font; }
		
		inline const Vector2& getComputedSize() const { return m_size; }
		
	private:
		std::string m_text;
		std::shared_ptr<const Font> m_font;
		std::vector<CharacterVertex> m_vertices;
		std::vector<Font::CharInfoUv> m_uv;
		Vector2 m_size;
		int m_wrapLength;
};

} // font
} // video
} // flat

#endif // FLAT_VIDEO_FONT_STRING_H



