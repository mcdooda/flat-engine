#ifndef FLAT_VIDEO_COLOR_H
#define FLAT_VIDEO_COLOR_H

#include <GL/glew.h>

namespace flat
{
namespace video
{

class Color
{
	public:
		typedef GLfloat AttributeType;
		
		Color() : r(1.f), g(1.f), b(1.f), a(1.f) {}
		Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
		Color(AttributeType r, AttributeType g, AttributeType b, AttributeType a) : r(r), g(g), b(b), a(a) {}
		explicit Color(AttributeType gray) : r(gray), g(gray), b(gray), a(1.f) {}
		
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


