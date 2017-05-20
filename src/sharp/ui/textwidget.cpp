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
	
}

void TextWidget::setText(const std::string& text)
{
	video::font::String::setText(text);
	Widget::m_computedSize = video::font::String::getComputedSize();
	Widget::m_size = m_computedSize;

	if (ui::Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void TextWidget::draw(const render::RenderSettings& renderSettings) const
{
	if (!m_visible || getVertices().empty())
		return;
	
	renderSettings.textureUniform.set(getFont()->getAtlasId());
	renderSettings.modelMatrixUniform.set(m_transform);
	
	renderSettings.colorUniform.set(m_textColor);
	renderSettings.secondaryColorUniform.set(video::Color::BLACK);
	renderSettings.textureGivenUniform.set(true);
	
	glEnableVertexAttribArray(renderSettings.positionAttribute);
	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getVertices()[0])));
	
	glEnableVertexAttribArray(renderSettings.uvAttribute);
	glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getUv()[0])));
	
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(getUv().size()));
	
	glDisableVertexAttribArray(renderSettings.positionAttribute);
	glDisableVertexAttribArray(renderSettings.uvAttribute);
}

} // ui
} // sharp
} // flat
