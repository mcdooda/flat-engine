#ifndef FLAT_SHARP_UI_CANVASWIDGET_H
#define FLAT_SHARP_UI_CANVASWIDGET_H

#include "sharp/ui/widget.h"
#include "sharp/ui/layouts/fixedlayout.h"

#include "video/framebuffer.h"
#include "render/programsettings.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class CanvasWidget : public WidgetImpl<FixedLayout>
{
	using Super = WidgetImpl<FixedLayout>;
	public:
		CanvasWidget(std::shared_ptr<const render::ProgramSettings> render, const Vector2& size);
		~CanvasWidget();

		void layoutDone() override;

		void clear(const video::Color& clearColor);

		void drawLine(const video::Color& color, float width, bool smoothLine, const Vector2& from, const Vector2& to);
		void drawBezier(const video::Color& color, float width, bool smoothLine, const std::vector<Vector2>& controlPoints);

	private:
		void drawLines(const video::Color& color, float width, bool smoothLine, const Vector2* vertices, GLsizei count);

		bool updateCanvasSize();

	private:
		std::unique_ptr<video::FrameBuffer> m_frameBuffer;
		std::shared_ptr<const render::ProgramSettings> m_render;
		std::shared_ptr<const video::Texture> m_backgroundTexture;
		Matrix4 m_viewMatrix;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_CANVASWIDGET_H

