#include "video/view.h"
#include "video/window.h"

namespace flat
{
namespace video
{

View::View() :
	m_window(nullptr)
{

}

View::View(const Window* window) :
	m_window(window)
{
	
}

void View::reset()
{
	m_viewMatrix = Matrix4();
}

void View::zoom(float factor)
{
	scaleBy(m_viewMatrix, flat::Vector3(factor, factor, 1.f));
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

flat::Vector2 View::getRelativePosition(const Vector2& windowPosition) const
{
	FLAT_ASSERT_MSG(m_window != nullptr, "Must set window before calling getRelativePosition()");
	Matrix4 matrix = inverse(getViewProjectionMatrix());
	const Vector2& windowSize = m_window->getSize();
	Vector2 position = Vector2(
		(windowPosition.x / windowSize.x) * 2.f - 1.f,
		(windowPosition.y / windowSize.y) * 2.f - 1.f
	);
	return Vector2(matrix * Vector4(position, 0.f, 1.f));
}

Vector2 View::getWindowPosition(const Vector2& relativePosition) const
{
	FLAT_ASSERT_MSG(m_window != nullptr, "Must set window before calling getWindowPosition()");
	Vector2 position(getViewProjectionMatrix() * Vector4(relativePosition, 0.f, 1.f));
	const Vector2& windowSize = m_window->getSize();
	position.x = (position.x + 1.f) / 2.f * windowSize.x;
	position.y = (position.y + 1.f) / 2.f * windowSize.y;
	return position;
}

void View::updateProjection()
{
	FLAT_ASSERT_MSG(m_window != nullptr, "Must set window before calling updateProjection()");
	const Vector2& windowSize = m_window->getSize();
	float halfWidth = windowSize.x / 2.f;
	float halfHeight = windowSize.y / 2.f;
	m_projectionMatrix = ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -10000.0f, 10000.0f);
}

void View::getScreenAABB(flat::AABB2& aabb) const
{
	aabb.min = getRelativePosition(flat::Vector2(0, 0));
	aabb.max = getRelativePosition(m_window->getSize());
	if (aabb.min.x > aabb.max.x)
	{
		std::swap(aabb.min.x, aabb.max.x);
	}
	if (aabb.min.y > aabb.max.y)
	{
		std::swap(aabb.min.y, aabb.max.y);
	}
}

} // video
} // flat


