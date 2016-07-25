#include "textwidget.h"
#include "../../util/rendersettings.h"

namespace flat
{
namespace sharp
{
namespace ui
{

TextWidget::TextWidget(std::shared_ptr<const flat::video::font::Font> font) : flat::video::font::String(font)
{
	
}

void TextWidget::setText(const std::string& text)
{
	flat::video::font::String::setText(text);
	const flat::geometry::Vector2& size = getComputedSize();
	setSize(size);
}

void TextWidget::draw(const util::RenderSettings& renderSettings) const
{
	if (!m_visible)
		return;
	
	renderSettings.textureUniform.setTexture(getFont()->getAtlasId());
	renderSettings.modelMatrixUniform.setMatrix4(m_transform);
	
	if (m_mouseOver || !click.on())
		renderSettings.colorUniform.setColor(video::Color::WHITE);
	else
		renderSettings.colorUniform.setColor(video::Color::BLACK);
	
	glEnableVertexAttribArray(renderSettings.positionAttribute);
	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getVertices()[0])));
	
	glEnableVertexAttribArray(renderSettings.uvAttribute);
	glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getUv()[0])));
	
	glDrawArrays(GL_TRIANGLES, 0, getUv().size());
	
	glDisableVertexAttribArray(renderSettings.positionAttribute);
	glDisableVertexAttribArray(renderSettings.uvAttribute);
}

} // ui
} // sharp
} // flat
