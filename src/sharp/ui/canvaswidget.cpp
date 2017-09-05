#include "canvaswidget.h"
#include "../../render/rendersettings.h"

namespace flat
{
namespace sharp
{
namespace ui
{

CanvasWidget::CanvasWidget(std::shared_ptr<const render::ProgramSettings> render, const Vector2& size) :
	m_render(render)
{
	m_computedSize = size;
	setSize(size);
	updateCanvasSize();
}

CanvasWidget::~CanvasWidget()
{
	
}

void CanvasWidget::layoutDone()
{
	updateCanvasSize();
	Super::layoutDone();
}

void CanvasWidget::clear(const video::Color& clearColor)
{
	m_frameBuffer->use();

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CanvasWidget::drawLine(const video::Color& color, float width, const Vector2& from, const Vector2& to)
{
	glDisable(GL_DEPTH_BUFFER);

	const render::ProgramSettings* render = m_render.get();

	m_frameBuffer->use();
	glUseProgram(render->program.getProgramId());

	const video::Uniform<Matrix4>& vpMatrixUniform = render->settings.viewProjectionMatrixUniform;
	vpMatrixUniform.set(m_viewMatrix);

	const video::Uniform<video::Color>& colorUniform = render->settings.colorUniform;
	colorUniform.set(color);
	
	const video::Attribute positionAttribute = render->settings.positionAttribute;

	glLineWidth(width);

	Vector2 vertices[2];
	vertices[0] = from;
	vertices[1] = to;

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), &vertices[0]);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(std::size(vertices)));

	glDisableVertexAttribArray(positionAttribute);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_BUFFER);
}

void CanvasWidget::updateCanvasSize()
{
	if (m_frameBuffer == nullptr || m_frameBuffer->getSize() != m_computedSize)
	{
		m_frameBuffer.reset(new video::FrameBuffer());
		m_frameBuffer->setSize(m_computedSize);
		m_backgroundTexture = m_frameBuffer->addTexture("Background");
		setBackground(m_backgroundTexture);
		m_viewMatrix = ortho(0.f, m_computedSize.x, m_computedSize.y, 0.f);
	}
}

} // ui
} // sharp
} // flat

