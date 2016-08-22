#ifndef FLAT_VIDEO_VIEW_H
#define FLAT_VIDEO_VIEW_H

#include "../misc/vector2.h"
#include "../misc/matrix4.h"

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
		void move(const Vector2& position);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		void flipY();
		
		Vector2 getRelativePosition(const Vector2& windowPosition, const Vector2& windowSize) const;

		void updateProjection(const Vector2& windowSize);
		
		inline const Matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		inline const Matrix4& getViewMatrix() const { return m_viewMatrix; }
		inline Matrix4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

	private:
		Matrix4 m_projectionMatrix;
		Matrix4 m_viewMatrix;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIEW_H


