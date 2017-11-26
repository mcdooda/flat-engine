#include <iterator>
#include <algorithm>
#include "string.h"

#include <iostream>

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

void String::setText(const std::string& text, const Color& color)
{
	size_t textLength = text.size();
	m_text = text;
	
	m_vertices.clear();
	m_vertices.reserve(textLength * 6);
	
	m_uv.clear();
	m_uv.reserve(textLength * 6);
	
	const Font* font = m_font.get();
	FLAT_ASSERT(font != nullptr);
	float characterHeight = font->getAtlasSize().y;
	
	float x = 0.f;
	float y = 0.f;
	for (size_t i = 0; i < textLength; ++i)
	{
		char c = text[i];
		
		if (c == '\n')
		{
			x = 0.f;
			y += characterHeight;
		}
		else
		{
			const Font::CharInfo& ci = font->getCharInfo(c);
			if (ci.visible)
			{
				float fx0 = x;
				float fx1 = x + ci.advance;
				float fy0 = y + characterHeight;
				float fy1 = y;
		
				m_vertices.emplace_back(fx0, fy0, color);
				m_vertices.emplace_back(fx1, fy0, color);
				m_vertices.emplace_back(fx0, fy1, color);
		
				m_vertices.emplace_back(fx1, fy1, color);
				m_vertices.emplace_back(fx0, fy1, color);
				m_vertices.emplace_back(fx1, fy0, color);
			
				std::copy(ci.uv.begin(), ci.uv.end(), std::back_inserter(m_uv));

				x += ci.advance;
			}
		}
	}
	m_size.x = x;
	m_size.y = y + characterHeight;
}

void String::setColor(int from, int to, const Color& color)
{
	FLAT_ASSERT(from >= 0 && from <= m_text.size());
	FLAT_ASSERT(to >= 0 && to <= m_text.size());
	FLAT_ASSERT(from <= to);
	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_vertices[i * 6 + j].color = color;
		}
	}
}

} // font
} // video
} // flat


