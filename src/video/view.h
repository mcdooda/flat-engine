#ifndef FLAT_VIDEO_VIEW_H
#define FLAT_VIDEO_VIEW_H

#include "../misc/vector.h"
#include "../misc/matrix4.h"

namespace flat
{
namespace video
{
class Window;

class View
{
	public:
		View();
		View(const Window* window);

		inline void setWindow(const Window* window) { m_window = window; }
		
		void reset();
		
		void zoom(float factor);
		void move(const Vector2& position);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		void flipY();
		
		// window to "2d world" position
		Vector2 getRelativePosition(const Vector2& windowPosition) const;

		// "2d world" to window position
		Vector2 getWindowPosition(const Vector2& relativePosition) const;

		void updateProjection();
		
		inline const Matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		inline const Matrix4& getViewMatrix() const { return m_viewMatrix; }
		inline Matrix4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

	private:
		Matrix4 m_projectionMatrix;
		Matrix4 m_viewMatrix;
		const Window* m_window;
};

} // video
} // flat

#endif // FLAT_VIDEO_VIEW_H


