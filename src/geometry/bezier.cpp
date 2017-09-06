#include "bezier.h"

namespace flat
{
namespace geometry
{
namespace bezier
{

namespace
{
static void computePointPosition(Vector2* result, int count, float coefficient)
{
	if (count == 1)
	{
		return;
	}

	for (int i = 0; i < count - 1; ++i)
	{
		result[i] = result[i] * (1.f - coefficient) + result[i + 1] * coefficient;
	}
	computePointPosition(result, count - 1, coefficient);
}

Vector2 computePointPosition(const std::vector<Vector2>& controlPoints, float coefficient)
{
	FLAT_ASSERT(controlPoints.size() > 1);

	Vector2* result = static_cast<Vector2*>(alloca(sizeof(Vector2) * (controlPoints.size() - 1)));
	for (int i = 0; i < controlPoints.size() - 1; ++i)
	{
		result[i] = controlPoints[i] * (1.f - coefficient) + controlPoints[i + 1] * coefficient;
	}
	computePointPosition(result, controlPoints.size() - 1, coefficient);
	return result[0];
}
}

void computeBezier(const std::vector<Vector2>& controlPoints, int numSteps, std::vector<Vector2>& bezierCurve)
{
	bezierCurve.reserve(numSteps);

	std::vector<Vector2> points;
	points.reserve(controlPoints.size());
	for (int i = 0; i < numSteps; ++i)
	{
		float coefficient = static_cast<float>(i) / (numSteps - 1);
		points = controlPoints;
		Vector2 point = computePointPosition(points, coefficient);
		bezierCurve.push_back(point);
	}
}

} // bezier
} // geometry
} // flat


