#ifndef FLAT_VIDEO_UNIFORM_H
#define FLAT_VIDEO_UNIFORM_H

#include "color.h"
#include "texture.h"
#include "../misc/vector3.h"
#include "../misc/matrix4.h"

namespace flat
{
namespace video
{

class BaseUniform
{
public:
	BaseUniform() : m_uniformLocation(0) {}
	BaseUniform(GLint uniformLocation) : m_uniformLocation(uniformLocation) {}

	inline bool isValid() const { return m_uniformLocation != 0; }

protected:
	GLint m_uniformLocation;
};

template <class T>
class Uniform : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}
};

template<>
class Uniform<bool> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	inline void set(bool value) const
	{
		glUniform1i(m_uniformLocation, value);
	}
};

template<>
class Uniform<int> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	inline void set(int value) const
	{
		glUniform1i(m_uniformLocation, value);
	}
};

template<>
class Uniform<float> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	inline void set(float value) const
	{
		glUniform1f(m_uniformLocation, value);
	}
};

template<>
class Uniform<Color> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	inline void set(const Color& value) const
	{
		glUniform4f(m_uniformLocation, value.r, value.g, value.b, value.a);
	}
};

template<>
class Uniform<Vector2> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	void set(const Vector2& vector2) const
	{
		glUniform2f(m_uniformLocation, vector2.x, vector2.y);
	}
};

template<>
class Uniform<Vector3> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	void set(const Vector3& vector3) const
	{
		glUniform3f(m_uniformLocation, vector3.x, vector3.y, vector3.z);
	}
};

template<>
class Uniform<Matrix4> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	void set(const Matrix4& matrix4) const
	{
		glUniformMatrix4fv(m_uniformLocation, 1, GL_TRUE, matrix4.getData());
	}
};

template<>
class Uniform<Texture> : public BaseUniform
{
public:
	Uniform() : BaseUniform() {}
	Uniform(GLint uniformLocation) : BaseUniform(uniformLocation) {}

	void set(GLuint textureId, int index = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(m_uniformLocation, index);
	}

	void set(const Texture* texture, int index = 0) const
	{
		set(texture->getTextureId(), index);
	}
};

} // video
} // flat

#endif // FLAT_VIDEO_UNIFORM_H


