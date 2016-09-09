#include "view.h"
#include "window.h"

namespace flat
{
namespace video
{

View::View()
{
	
}

void View::reset()
{
	m_viewMatrix = Matrix4();
}

void View::zoom(float factor)
{
	scaleBy(m_viewMatrix, factor);
}

void View::move(const Vector2& move)
{
	translateBy(m_viewMatrix, Vector3(-move, 0.f));
}

void View::rotateX(float angle)
{
	rotateXBy(m_viewMatrix, angle);
}

void View::rotateY(float angle)
{
	rotateYBy(m_viewMatrix, angle);
}

void View::rotateZ(float angle)
{
	rotateZBy(m_viewMatrix, angle);
}

void View::flipY()
{
	scaleBy(m_viewMatrix, Vector2(1.f, -1.f));
}

flat::Vector2 View::getRelativePosition(const Vector2& windowPosition, const Vector2& windowSize) const
{
	Matrix4 matrix = inverse(getViewProjectionMatrix());
	Vector2 position = Vector2(
		(windowPosition.x / windowSize.x) * 2.f - 1.f,
		(windowPosition.y / windowSize.y) * 2.f - 1.f
	);
	return Vector2(matrix * Vector4(position, 0.f, 1.f));
}

void View::updateProjection(const Vector2& windowSize)
{
	float halfWidth = windowSize.x / 2.f;
	float halfHeight = windowSize.y / 2.f;
	m_projectionMatrix = ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1000.0f, 1000.0f);
}

} // video
} // flat


