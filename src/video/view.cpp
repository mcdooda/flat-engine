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
	m_viewMatrix.setIdentity();
}

void View::zoom(float factor)
{
	m_viewMatrix.scale(factor);
}

void View::move(const Vector2& move)
{
	m_viewMatrix.translate(move);
}

void View::rotateX(float angle)
{
	m_viewMatrix.rotateX(angle);
}

void View::rotateY(float angle)
{
	m_viewMatrix.rotateY(angle);
}

void View::rotateZ(float angle)
{
	m_viewMatrix.rotateZ(angle);
}

void View::flipY()
{
	m_viewMatrix.scale(1.f, -1.f);
}

flat::Vector2 View::getRelativePosition(const Vector2& windowPosition, const Vector2& windowSize) const
{
	Matrix4 matrix = getViewProjectionMatrix();
	matrix.setInverse();
	Vector2 position = Vector2(
		(windowPosition.x / windowSize.x) * 2.f - 1.f,
		(windowPosition.y / windowSize.y) * 2.f - 1.f
	);
	return matrix * position;
}

void View::updateProjection(const Vector2& windowSize)
{
	float halfWidth = windowSize.x / 2.f;
	float halfHeight = windowSize.y / 2.f;
	m_projectionMatrix.setOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1000.0f, 1000.0f);
}

} // video
} // flat


