#include "textwidget.h"
#include "../../render/rendersettings.h"

namespace flat
{
namespace sharp
{
namespace ui
{

TextWidget::TextWidget(const std::shared_ptr<const video::font::Font>& font) : video::font::String(font)
{
	setText("");
}

void TextWidget::setText(const std::string& text)
{
	video::font::String::setText(text, m_textColor);
	Widget::m_computedSize = video::font::String::getComputedSize();
	Widget::m_size = m_computedSize;

	if (ui::Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void TextWidget::setTextColor(const video::Color& textColor)
{
	String::setColor(0, static_cast<int>(String::getText().size()), textColor);
	m_textColor = textColor;
}

void TextWidget::draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	if (!m_visible || getVertices().empty())
	{
		return;
	}

	ScissorRectangle scissor;
	if (!computeAndApplyScissor(scissor, parentScissor))
	{
		return;
	}
	
	renderSettings.textureUniform.set(getFont()->getAtlasId());
	renderSettings.modelMatrixUniform.set(m_transform);
	renderSettings.vertexColorGivenUniform.set(true);
	
	renderSettings.secondaryColorUniform.set(video::Color::BLACK);
	renderSettings.textureGivenUniform.set(true);
	
	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), reinterpret_cast<const float*>(&(getVertices()[0])));

	glEnableVertexAttribArray(renderSettings.colorAttribute);
	glVertexAttribPointer(renderSettings.colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), reinterpret_cast<const float*>(&(getVertices()[0].color)));
	
	glEnableVertexAttribArray(renderSettings.uvAttribute);
	glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getUv()[0])));
	
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(getUv().size()));
	
	glDisableVertexAttribArray(renderSettings.colorAttribute);
	glDisableVertexAttribArray(renderSettings.uvAttribute);
}

} // ui
} // sharp
} // flat
