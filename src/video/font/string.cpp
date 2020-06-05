#include <iterator>
#include <algorithm>

#include "video/font/string.h"

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

	if (textLength == 0)
	{
		m_size = flat::Vector2(0.f, font->getHeight());
		return;
	}

	const float lineSkip = font->getLineSkip();
	const size_t nbLines = std::count(text.begin(), text.end(), '\n') + 1;
	m_size.y = (nbLines - 1) * lineSkip + font->getHeight();

	const float ascent = font->getAscent();

	float maxX = 0.f;
	float x = 0.f;
	float y = m_size.y;
	for (size_t i = 0; i < textLength; ++i)
	{
		char c = text[i];
		if (c == '\n')
		{
			if(x > maxX)
				maxX = x;
			x = 0.f;
			y -= lineSkip;
		}
		else
		{
			const Font::CharInfo& ci = font->getCharInfo(c);
			if (ci.visible)
			{
				const float fx0 = x + ci.minX;
				const float fx1 = x + ci.maxX;
				const float fy0 = y + ci.maxY - ascent;
				const float fy1 = y + ci.minY - ascent;

				m_vertices.emplace_back(fx0, fy0, color);
				m_vertices.emplace_back(fx1, fy0, color);
				m_vertices.emplace_back(fx0, fy1, color);

				m_vertices.emplace_back(fx1, fy1, color);
				m_vertices.emplace_back(fx0, fy1, color);
				m_vertices.emplace_back(fx1, fy0, color);

				std::copy(ci.uv.begin(), ci.uv.end(), std::back_inserter(m_uv));

			}
			x += ci.advance;
		}
	}
	m_size.x = std::max(maxX, x);
}

void String::addBackgroundRectangle()
{
	if (m_vertices.empty())
	{
		return;
	}

	const Color& color = m_vertices[0].color;

	const float fx0 = m_vertices[0].x;
	const float fx1 = fx0 + m_size.x;
	const float fy0 = m_vertices[0].y - m_font->getAscent() + m_font->getLineSkip();
	const float fy1 = fy0 - m_size.y;

	m_vertices.insert(m_vertices.begin(), 6, CharacterVertex(0.f, 0.f, color));

	m_vertices[0].x = fx0;
	m_vertices[0].y = fy0;

	m_vertices[1].x = fx1;
	m_vertices[1].y = fy0;

	m_vertices[2].x = fx0;
	m_vertices[2].y = fy1;


	m_vertices[3].x = fx1;
	m_vertices[3].y = fy1;

	m_vertices[4].x = fx0;
	m_vertices[4].y = fy1;

	m_vertices[5].x = fx1;
	m_vertices[5].y = fy0;

	m_uv.insert(m_uv.begin(), 6, Font::CharInfoUv(0.f, 0.f));
}

void String::setColor(unsigned int from, unsigned int to, const Color& color)
{
	FLAT_ASSERT(from >= 0 && from <= m_text.size());
	FLAT_ASSERT(to >= 0 && to <= m_text.size());
	FLAT_ASSERT(from <= to);

	if (from == 0 && to == m_text.size())
	{
		for (CharacterVertex& characterVertex : m_vertices)
		{
			characterVertex.color = color;
		}
		return;
	}

	const unsigned int nbLinesFrom = static_cast<unsigned int>(std::count(getText().begin(), getText().begin() + from, '\n'));
	const unsigned int nbLinesTo = static_cast<unsigned int>(std::count(getText().begin() + from, getText().begin() + to, '\n'));
	from -= nbLinesFrom;
	to -= nbLinesFrom + nbLinesTo;
	for (unsigned int i = from; i < to; i++)
	{
		for (unsigned int j = 0; j < 6; j++)
		{
			m_vertices[i * 6 + j].color = color;
		}
	}
}

} // font
} // video
} // flat


