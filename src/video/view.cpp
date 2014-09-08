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

void View::move(geometry::Vector2 move)
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

flat::geometry::Vector2 View::getRelativePosition(const geometry::Vector2& windowPosition, const geometry::Vector2& windowSize) const
{
	geometry::Matrix4 matrix = getViewProjectionMatrix();
	matrix.setInverse();
	geometry::Vector2 position = geometry::Vector2(
		(windowPosition.getX() / windowSize.getX()) * 2 - 1,
		(windowPosition.getY() / windowSize.getY()) * 2 - 1
	);
	return matrix * position;
}

void View::updateProjection(const geometry::Vector2& windowSize)
{
	float halfWidth = windowSize.getX() / 2;
	float halfHeight = windowSize.getY() / 2;
	m_projectionMatrix.setOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1000.0f, 1000.0f);
}

geometry::Matrix4 View::getNormalMatrix() const
{
	geometry::Matrix4 normalMatrix = m_viewMatrix;
	normalMatrix.setInverseTranspose();
	return normalMatrix;
}

} // video
} // flat


