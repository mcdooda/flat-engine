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
	m_viewMatrix = glm::mat4();
}

void View::zoom(float factor)
{
	m_viewMatrix = glm::scale(m_viewMatrix, glm::vec3(factor, factor, factor));
}

void View::move(const Vector2& move)
{
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(move, 0.f));
}

void View::rotateX(float angle)
{
	m_viewMatrix = glm::rotate(m_viewMatrix, angle, glm::vec3(1.f, 0.f, 0.f));
}

void View::rotateY(float angle)
{
	m_viewMatrix = glm::rotate(m_viewMatrix, angle, glm::vec3(0.f, 1.f, 0.f));
}

void View::rotateZ(float angle)
{
	m_viewMatrix = glm::rotate(m_viewMatrix, angle, glm::vec3(0.f, 0.f, 1.f));
}

void View::flipY()
{
	m_viewMatrix = glm::scale(m_viewMatrix, glm::vec3(1.f, -1.f, 1.f));
}

flat::Vector2 View::getRelativePosition(const Vector2& windowPosition, const Vector2& windowSize) const
{
	Matrix4 matrix = glm::inverse(getViewProjectionMatrix());
	Vector2 position = Vector2(
		(windowPosition.x / windowSize.x) * 2.f - 1.f,
		(windowPosition.y / windowSize.y) * 2.f - 1.f
	);
	return Vector2(matrix * glm::vec4(position, 0.f, 1.f));
}

void View::updateProjection(const Vector2& windowSize)
{
	float halfWidth = windowSize.x / 2.f;
	float halfHeight = windowSize.y / 2.f;
	m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1000.0f, 1000.0f);
}

} // video
} // flat


