#ifndef FLAT_VIDEO_COLOR_H
#define FLAT_VIDEO_COLOR_H

#include <cstdint>
#include <GL/glew.h>

namespace flat
{
namespace video
{

class Color
{
	public:
		using AttributeType = GLfloat;
		
		Color() :
			r(1.f),
			g(1.f),
			b(1.f),
			a(1.f)
		{

		}

		Color(const Color& color) :
			r(color.r),
			g(color.g),
			b(color.b),
			a(color.a)
		{

		}

		Color(AttributeType r, AttributeType g, AttributeType b, AttributeType a) :
			r(r),
			g(g),
			b(b),
			a(a)
		{

		}

		explicit Color(AttributeType gray) :
			r(gray),
			g(gray),
			b(gray),
			a(1.f)
		{

		}

		explicit Color(uint32_t rgba) :
			r(static_cast<float>((rgba >> 24) & 0xFF) / 255.f),
			g(static_cast<float>((rgba >> 16) & 0xFF) / 255.f),
			b(static_cast<float>((rgba >> 8 ) & 0xFF) / 255.f),
			a(static_cast<float>((rgba >> 0 ) & 0xFF) / 255.f)
		{}

		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
			r(static_cast<float>(r) / 255.f),
			g(static_cast<float>(g) / 255.f),
			b(static_cast<float>(b) / 255.f),
			a(static_cast<float>(a) / 255.f)
		{}

		operator uint32_t() const
		{
			return static_cast<uint32_t>(r * 255.f) << 24
			     | static_cast<uint32_t>(g * 255.f) << 16
			     | static_cast<uint32_t>(b * 255.f) << 8
			     | static_cast<uint32_t>(a * 255.f);
		}
		
		static Color WHITE;
		static Color BLACK;
		static Color RED;
		static Color GREEN;
		static Color BLUE;

	public:
		AttributeType r;
		AttributeType g;
		AttributeType b;
		AttributeType a;
};

} // video
} // flat

#endif // FLAT_VIDEO_COLOR_H


