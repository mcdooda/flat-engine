#include <iterator>
#include <algorithm>
#include "string.h"

namespace flat
{
namespace video
{
namespace font
{

String::String(const std::shared_ptr<const Font>& font) :
	m_font(font),
	m_wrapLength(0)
{
	
}

String::~String()
{
	
}

void String::setText(const std::string& text)
{
	size_t textLength = text.size();
	m_text = text;
	
	m_vertices.clear();
	m_vertices.reserve(textLength * 6);
	
	m_uv.clear();
	m_uv.reserve(textLength * 6);
	
	const Font* font = m_font.get();
	float characterHeight = font->getAtlasSize().y;
	
	float x = 0.f;
	float y = 0.f;
	for (int i = 0; i < textLength; ++i)
	{
		char c = text[i];
		const Font::CharInfo& ci = font->getCharInfo(c);
		
		if (ci.visible)
		{
			float fx0 = x;
			float fx1 = x + ci.advance;
			float fy0 = y + characterHeight;
			float fy1 = y;
		
			m_vertices.emplace_back(fx0, fy0);
			m_vertices.emplace_back(fx1, fy0);
			m_vertices.emplace_back(fx0, fy1);
		
			m_vertices.emplace_back(fx1, fy1);
			m_vertices.emplace_back(fx0, fy1);
			m_vertices.emplace_back(fx1, fy0);
			
			std::copy(ci.uv.getBuffer(), ci.uv.getBuffer() + 6, std::back_inserter(m_uv));
		}
		
		x += ci.advance;
	}
	m_size.x = x;
	m_size.y = y + characterHeight;
}

} // font
} // video
} // flat


