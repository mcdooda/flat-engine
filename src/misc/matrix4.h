#ifndef FLAT_GEOMETRY_MATRIX4_H
#define FLAT_GEOMETRY_MATRIX4_H

#include <iomanip>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace flat
{

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

} // flat

#endif // FLAT_GEOMETRY_MATRIX4_H


