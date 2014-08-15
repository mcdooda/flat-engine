#ifndef FLAT_VIDEO_VIEW_H
#define FLAT_VIDEO_VIEW_H

#include "../geometry/vector2.h"
#include "../geometry/matrix4.h"

namespace flat
{
namespace video
{

class View
{
	public:
		View();
		
		void reset();
		
		void zoom(float factor);
		void move(geometry::Vector2 position);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		
		geometry::Vector2 getRelativePosition(const geometry::Vector2& windowPosition, const geometry::Vector2& windowSize) const;

		void updateProjection(const geometry::Vector2& windowSize);
		
		inline const geometry::Matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		inline const geometry::Matrix4& getViewMatrix() const { return m_viewMatrix; }
		inline geometry::Matrix4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

	private:
		geometry::Matrix4 m_projectionMatrix;
		geometry::Matrix4 m_viewMatrix;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIEW_H


