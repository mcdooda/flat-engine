#ifndef FLAT_GEOMETRY_MATRIX4_H
#define FLAT_GEOMETRY_MATRIX4_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace flat
{

using Matrix4 = glm::mat4;

using glm::translate;
using glm::rotate;
using glm::scale;

using glm::inverse;
using glm::transpose;
using glm::ortho;
using glm::value_ptr;

} // flat

#endif // FLAT_GEOMETRY_MATRIX4_H


