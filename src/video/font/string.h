#ifndef FLAT_VIDEO_FONT_STRING_H
#define FLAT_VIDEO_FONT_STRING_H

#include <memory>
#include <string>
#include <vector>
#include "font.h"
#include "video\color.h"

namespace flat
{
namespace video
{
namespace font
{

class String
{
	protected:
		struct CharacterVertex
		{
			float x;
			float y;
			Color color;
			CharacterVertex(float x, float y) : x(x), y(y), color(Color::WHITE) {}
			CharacterVertex(float x, float y, const Color& color) : x(x), y(y), color(color){}
		};
	
	public:
		String() = delete;
		String(const String&) = delete;
		String(String&&) = delete;
		String(const std::shared_ptr<const Font>& font);
		~String() = default;
		String& operator=(const String&) = delete;
		
		inline void setWrapLength(int wrapLength) { m_wrapLength = wrapLength; }
		inline void setNoWrap() { setWrapLength(0); }
		
		void setText(const std::string& text, const Color& color = Color::WHITE);
		inline const std::string& getText() const { return m_text; }

		void setColor(unsigned int from, unsigned int to, const Color& style);
		
		inline const std::vector<CharacterVertex>& getVertices() const { return m_vertices; }
		inline const std::vector<Font::CharInfoUv>& getUv() const { return m_uv; }
		
		inline const std::shared_ptr<const Font>& getFont() const { return m_font; }
		
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



