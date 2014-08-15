#ifndef ENGINE_VIDEO_COLOR_H
#define ENGINE_VIDEO_COLOR_H

#include <GL/glew.h>

namespace engine
{
namespace video
{

class Color
{
	public:
		Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		Color(GLfloat gray);

		inline GLfloat getR() const { return m_r; }
		inline GLfloat getG() const { return m_g; }
		inline GLfloat getB() const { return m_b; }
		inline GLfloat getA() const { return m_a; }

		inline void setR(GLfloat r) { m_r = r; }
		inline void setG(GLfloat g) { m_g = g; }
		inline void setB(GLfloat b) { m_b = b; }
		inline void setA(GLfloat a) { m_a = a; }
		
		Color* newCopy();
		
		static Color WHITE;
		static Color BLACK;

	protected:
		GLfloat m_r;
		GLfloat m_g;
		GLfloat m_b;
		GLfloat m_a;
};

} // video
} // engine

#endif // ENGINE_VIDEO_COLOR_H


