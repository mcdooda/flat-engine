#ifndef FLAT_GEOMETRY_MATRIX4_H
#define FLAT_GEOMETRY_MATRIX4_H

#include <iomanip>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace flat
{

using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;

inline std::ostream& operator<<(std::ostream& out, const Matrix4& m)
{
	const int w = 8;
	const int p = 3;

	std::streamsize precision = out.precision();

	out.precision(p);

	for (int i = 0; i < 4; ++i)
	{
		out << "("
			<< std::setw(w) << m[0][i] << " "
			<< std::setw(w) << m[1][i] << " "
			<< std::setw(w) << m[2][i] << " "
			<< std::setw(w) << m[3][i]
			<< ")" << std::endl;
	}

	out.precision(precision);

	return out;
}

using glm::translate;
using glm::rotate;
using glm::scale;

using glm::inverse;
using glm::transpose;
using glm::ortho;
using glm::value_ptr;

inline void transformBy(Matrix4& transform, const Matrix4& other)
{
	transform = other * transform;
}

inline void translateBy(Matrix4& transform, const Vector2& translation)
{
	transform = translate(transform, Vector3(translation, 0.f));
}

inline void translateBy(Matrix4& transform, const Vector3& translation)
{
	transform = translate(transform, translation);
}

inline void rotateXBy(Matrix4& transform, float rotation)
{
	transform = rotate(transform, rotation, Vector3(1.f, 0.f, 0.f));
}

inline void rotateYBy(Matrix4& transform, float rotation)
{
	transform = rotate(transform, rotation, Vector3(0.f, 1.f, 0.f));
}

inline void rotateZBy(Matrix4& transform, float rotation)
{
	transform = rotate(transform, rotation, Vector3(0.f, 0.f, 1.f));
}

inline void rotateBy(Matrix4& transform, const Vector3& rotation)
{
	rotateZBy(transform, rotation.z);
	rotateYBy(transform, rotation.y);
	rotateXBy(transform, rotation.x);
}

inline void scaleBy(Matrix4& transform, float scale)
{
	transform = flat::scale(transform, Vector3(scale, scale, scale));
}

inline void scaleBy(Matrix4& transform, const Vector2& scale)
{
	transform = flat::scale(transform, Vector3(scale, 1.f));
}

inline void scaleBy(Matrix4& transform, const Vector3& scale)
{
	transform = flat::scale(transform, scale);
}

} // flat

#endif // FLAT_GEOMETRY_MATRIX4_H


