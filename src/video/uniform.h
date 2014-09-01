#ifndef FLAT_VIDEO_UNIFORM_H
#define FLAT_VIDEO_UNIFORM_H

#include "color.h"
#include "texture.h"
#include "../geometry/vector2.h"
#include "../geometry/matrix4.h"

namespace flat
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
		void setTexture(const Texture* texture, int i = 0) const;
		void setVector2(const geometry::Vector2& vector2) const;
		void setMatrix4(const geometry::Matrix4& matrix4) const;
		
	private:
		GLint m_uniformLocation;
};

} // video
} // flat

#endif // FLAT_VIDEO_UNIFORM_H


