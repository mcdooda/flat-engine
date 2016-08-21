#include <GL/glew.h>
#include "uniform.h"

namespace flat
{
namespace video
{

Uniform::Uniform(GLint uniformLocation) :
	m_uniformLocation(uniformLocation)
{
	
}

Uniform::Uniform() :
	m_uniformLocation(0)
{
	
}

Uniform::~Uniform()
{
	
}

void Uniform::setBool(bool b) const
{
	glUniform1i(m_uniformLocation, b);
}

void Uniform::setInt(int i) const
{
	glUniform1i(m_uniformLocation, i);
}

void Uniform::setFloat(float f) const
{
	glUniform1f(m_uniformLocation, f);
}

void Uniform::setColor(const Color& color) const
{
	glUniform4f(m_uniformLocation, color.r, color.g, color.b, color.a);
}

void Uniform::setTexture(const Texture* texture, int i) const
{
	setTexture(texture->getTextureId(), i);
}

void Uniform::setTexture(GLuint textureId, int i) const
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, textureId);
	setInt(i);
}

void Uniform::setVector2(const geometry::Vector2& vector2) const
{
	glUniform2f(m_uniformLocation, vector2.x, vector2.y);
}

void Uniform::setVector3(const geometry::Vector3& vector3) const
{
	glUniform3f(m_uniformLocation, vector3.x, vector3.y, vector3.z);
}

void Uniform::setMatrix4(const geometry::Matrix4& matrix4) const
{
	glUniformMatrix4fv(m_uniformLocation, 1, GL_TRUE, matrix4.getData());
}

} // video
} // flat


