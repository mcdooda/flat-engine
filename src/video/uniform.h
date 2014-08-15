#ifndef ENGINE_VIDEO_UNIFORM_H
#define ENGINE_VIDEO_UNIFORM_H

#include "color.h"
#include "../geometry/vector2.h"
#include "../geometry/matrix4.h"

namespace engine
{
namespace video
{

class Uniform
{
	public:
		Uniform(GLint uniformLocation);
		Uniform();
		~Uniform();
		
		void setInt(int i) const;
		void setFloat(float f) const;
		void setColor(const Color& color) const;
		void setVector2(const geometry::Vector2& vector2) const;
		void setMatrix4(const geometry::Matrix4& matrix4) const;
		
	private:
		GLint m_uniformLocation;
};

} // video
} // engine

#endif // ENGINE_VIDEO_UNIFORM_H


