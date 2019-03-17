#include "sharp/ui/canvaswidget.h"

#include "render/rendersettings.h"
#include "geometry/bezier.h"

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
	if (updateCanvasSize())
	{
		Super::layoutDone();
	}
}

void CanvasWidget::clear(const video::Color& clearColor)
{
	m_frameBuffer->use();

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CanvasWidget::drawLine(const video::Color& color, float width, bool smoothLine, const Vector2& from, const Vector2& to)
{
	Vector2 vertices[2];
	vertices[0] = from;
	vertices[1] = to;
	drawLines(color, width, smoothLine, vertices, 2);
}

void CanvasWidget::drawBezier(const video::Color& color, float width, bool smoothLine, const std::vector<Vector2>& controlPoints)
{
	FLAT_ASSERT(controlPoints.size() > 1);
	std::vector<Vector2> bezierCurve;
	float length = 0.f;
	for (int i = 0; i < controlPoints.size() - 1; ++i)
	{
		length += flat::length(controlPoints[i] - controlPoints[i + 1]);
	}
	int numSteps = std::max(2, static_cast<int>(length / 10.f));
	geometry::bezier::computeBezier(controlPoints, numSteps, bezierCurve);
	drawLines(color, width, smoothLine, bezierCurve.data(), static_cast<GLsizei>(bezierCurve.size()));
}

void CanvasWidget::drawLines(const video::Color& color, float width, bool smoothLine, const Vector2* vertices, GLsizei count)
{
	glDisable(GL_DEPTH_BUFFER);
	if (smoothLine)
	{
		glEnable(GL_LINE_SMOOTH);
	}

	const render::ProgramSettings* render = m_render.get();

	m_frameBuffer->use();
	glUseProgram(render->program.getProgramId());

	const video::Uniform<Matrix4>& vpMatrixUniform = render->settings.viewProjectionMatrixUniform;
	vpMatrixUniform.set(m_viewMatrix);

	const video::Uniform<video::Color>& colorUniform = render->settings.colorUniform;
	colorUniform.set(color);

	const video::Attribute positionAttribute = render->settings.positionAttribute;

	glLineWidth(width);

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), &vertices[0]);

	glDrawArrays(GL_LINE_STRIP, 0, count);

	glDisableVertexAttribArray(positionAttribute);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_BUFFER);
	if (smoothLine)
	{
		glDisable(GL_LINE_SMOOTH);
	}
}

bool CanvasWidget::updateCanvasSize()
{
	if (m_frameBuffer == nullptr || m_frameBuffer->getSize() != m_computedSize)
	{
		m_frameBuffer.reset(new video::FrameBuffer());
		m_frameBuffer->setSize(m_computedSize);
		m_backgroundTexture = m_frameBuffer->addTexture("Background");
		setBackground(m_backgroundTexture);
		m_viewMatrix = ortho(0.f, m_computedSize.x, m_computedSize.y, 0.f);
		return true;
	}
	return false;
}

} // ui
} // sharp
} // flat

