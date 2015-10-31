#include <limits>
#include <algorithm>
#include "widget.h"
#include "../../util/rendersettings.h"
#include "../../video/color.h"
#include "../../video/filetexture.h"
#include "../../memory/memory.h"

namespace flat
{
namespace sharp
{
namespace ui
{

Widget::Widget() :
	m_margin(0.f),
	m_padding(0.f),
	m_size(0.f, 0.f),
	m_position(0.f, 0.f),
	m_rotation(0.f, 0.f, 0.f),
	m_backgroundRepeat(BackgroundRepeat::SCALED),
	m_sizePolicy(SizePolicy::COMPRESS),
	m_positionPolicy(PositionPolicy::TOP_LEFT),
	m_mouseOver(false),
	m_parent(nullptr)
{
	
}

Widget::~Widget()
{
	for (Widget* child : m_children)
	{
		FLAT_DELETE(child);
	}
}

void Widget::setSizePolicy(SizePolicy sizePolicy)
{
	setSizePolicyX(static_cast<SizePolicy>(sizePolicy & (SizePolicy::COMPRESS_X | SizePolicy::EXPAND_X | SizePolicy::FIXED_X)));
	setSizePolicyY(static_cast<SizePolicy>(sizePolicy & (SizePolicy::COMPRESS_Y | SizePolicy::EXPAND_Y | SizePolicy::FIXED_Y)));
}

void Widget::setSizePolicyX(SizePolicy sizePolicyX)
{
	FLAT_ASSERT(
		   ((sizePolicyX & SizePolicy::COMPRESS_X) && ((sizePolicyX & SizePolicy::EXPAND_X) | (sizePolicyX & SizePolicy::FIXED_X)) == 0)
		|| ((sizePolicyX & SizePolicy::EXPAND_X) && ((sizePolicyX & SizePolicy::COMPRESS_X) | (sizePolicyX & SizePolicy::FIXED_X)) == 0)
		|| ((sizePolicyX & SizePolicy::FIXED_X) && ((sizePolicyX & SizePolicy::COMPRESS_X) | (sizePolicyX & SizePolicy::EXPAND_X)) == 0)
	);
	
	m_sizePolicy = static_cast<SizePolicy>((m_sizePolicy & ~(SizePolicy::COMPRESS_X | SizePolicy::EXPAND_X | SizePolicy::FIXED_X)) | sizePolicyX);
}

void Widget::setSizePolicyY(SizePolicy sizePolicyY)
{
	FLAT_ASSERT(
		   ((sizePolicyY & SizePolicy::COMPRESS_Y) && ((sizePolicyY & SizePolicy::EXPAND_Y) | (sizePolicyY & SizePolicy::FIXED_Y)) == 0)
		|| ((sizePolicyY & SizePolicy::EXPAND_Y) && ((sizePolicyY & SizePolicy::COMPRESS_Y) | (sizePolicyY & SizePolicy::FIXED_Y)) == 0)
		|| ((sizePolicyY & SizePolicy::FIXED_Y) && ((sizePolicyY & SizePolicy::COMPRESS_Y) | (sizePolicyY & SizePolicy::EXPAND_Y)) == 0)
	);
	
	m_sizePolicy = static_cast<SizePolicy>((m_sizePolicy & ~(SizePolicy::COMPRESS_Y | SizePolicy::EXPAND_Y | SizePolicy::FIXED_Y)) | sizePolicyY);
}

void Widget::setPositionPolicy(PositionPolicy positionPolicy)
{
	FLAT_ASSERT(!((positionPolicy & PositionPolicy::FLOW_X) && (positionPolicy & PositionPolicy::FLOW_Y)));
	m_positionPolicy = positionPolicy;
}

void Widget::addChild(Widget* widget)
{
	FLAT_ASSERT(!widget->m_parent);
	m_children.push_back(widget);
	widget->m_parent = this;
}

void Widget::draw(const util::RenderSettings& renderSettings) const
{
	const video::FileTexture* background = m_background.get();
	if (background != nullptr)
	{
		renderSettings.textureUniform.setTexture(background);
		renderSettings.modelMatrixUniform.setMatrix4(m_transform);

		if (m_mouseOver)
			renderSettings.colorUniform.setColor(video::Color::WHITE);
		else
			renderSettings.colorUniform.setColor(video::Color::BLACK);

		// enable vertex attrib array
		// position
		const float position[] = {
			m_position.x,            m_position.y,
			m_position.x + m_size.x, m_position.y,
			m_position.x + m_size.x, m_position.y + m_size.y,
			m_position.x,            m_position.y + m_size.y
		};

		glEnableVertexAttribArray(renderSettings.positionAttribute);
		glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, position);

		// uv
		const float* uv = nullptr;

		// TODO compute during layout...
		const float repeatUv[] = {
			0.0f, m_size.y / background->getSize().y,
			m_size.x / background->getSize().x, m_size.y / background->getSize().y,
			m_size.x / background->getSize().x, 0.0f,
			0.0f, 0.0f
		};

		static const float scaledUv[] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};

		if (m_backgroundRepeat == BackgroundRepeat::REPEAT)
		{
			uv = repeatUv;
		}
		else if (m_backgroundRepeat == BackgroundRepeat::SCALED)
		{
			uv = scaledUv;
		}
		else
		{
			FLAT_ASSERT(false);
		}

		glEnableVertexAttribArray(renderSettings.uvAttribute);
		glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, uv);

		// draw
		glDrawArrays(GL_QUADS, 0, 4);

		// disable vertex attrib array
		glDisableVertexAttribArray(renderSettings.positionAttribute);
		glDisableVertexAttribArray(renderSettings.uvAttribute);

	}

	drawChildren(renderSettings);
}

bool Widget::isInside(const geometry::Vector2& point) const
{
	geometry::Matrix4 invTransform = m_transform;
	invTransform.setInverse();
	geometry::Vector2 localPoint = invTransform * point;
	return localPoint.x >= 0 && localPoint.x <= m_size.x
			&& localPoint.y >= 0 && localPoint.y <= m_size.y;
}

void Widget::onMouseEnter()
{
	m_mouseOver = true;
}

void Widget::onMouseLeave()
{
	m_mouseOver = false;
}

void Widget::drawChildren(const util::RenderSettings& renderSettings) const
{
	for (Widget* child : m_children)
	{
		child->draw(renderSettings);
	}
}

Widget* Widget::getMouseOverWidget(const geometry::Vector2& mousePosition)
{
	if (isInside(mousePosition))
	{
		for (Widget* child : m_children)
		{
			if (Widget* overWidget = child->getMouseOverWidget(mousePosition))
				return overWidget;
		}
		return this;
	}
	return nullptr;
}

} // ui
} // sharp
} // flat



