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
		
		Color();
		Color(const Color& color);
		Color(AttributeType r, AttributeType g, AttributeType b, AttributeType a);
		explicit Color(AttributeType gray);

		inline AttributeType getR() const { return m_r; }
		inline AttributeType getG() const { return m_g; }
		inline AttributeType getB() const { return m_b; }
		inline AttributeType getA() const { return m_a; }

		inline void setR(AttributeType r) { m_r = r; }
		inline void setG(AttributeType g) { m_g = g; }
		inline void setB(AttributeType b) { m_b = b; }
		inline void setA(AttributeType a) { m_a = a; }
		
		static Color WHITE;
		static Color BLACK;
		static Color RED;
		static Color GREEN;
		static Color BLUE;

	protected:
		AttributeType m_r;
		AttributeType m_g;
		AttributeType m_b;
		AttributeType m_a;
};

} // video
} // flat

#endif // FLAT_VIDEO_COLOR_H


