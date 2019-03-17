#ifndef FLAT_GEOMETRY_BEZIER_H
#define FLAT_GEOMETRY_BEZIER_H

#include <vector>

#include "misc/vector.h"

namespace flat
{
namespace geometry
{
namespace bezier
{

void computeBezier(const std::vector<Vector2>& controlPoints, int numSteps, std::vector<Vector2>& bezierCurve);

} // bezier
} // geometry
} // flat

#endif // FLAT_GEOMETRY_BEZIER_H