#include <limits>
#include <algorithm>
#include "widget.h"
#include "rootwidget.h"
#include "layouts/fixedlayout.h"
#include "../../render/rendersettings.h"
#include "../../video/color.h"
#include "../../video/texture.h"
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
	m_backgroundPosition(0.f, 0.f),
	m_backgroundRepeat(BackgroundRepeat::SCALED),
	m_backgroundColor(0.f, 0.f, 0.f, 0.f),
	m_sizePolicy(SizePolicy::FIXED),
	m_positionPolicy(PositionPolicy::TOP_LEFT),
	m_mouseOver(false),
	m_visible(true)
{

}

Widget::~Widget()
{

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

void Widget::setSize(const Size& size)
{
	m_size = size;

	if (Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void Widget::setPositionPolicy(PositionPolicy positionPolicy)
{
	FLAT_ASSERT(!((positionPolicy & PositionPolicy::FLOW_X) && (positionPolicy & PositionPolicy::FLOW_Y)));
	m_positionPolicy = positionPolicy;
}

void Widget::setPosition(const Position& position)
{
	m_position = position;

	if (Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void Widget::addChild(const std::shared_ptr<Widget>& widget)
{
	FLAT_ASSERT(widget->m_parent.expired());
	m_children.push_back(std::shared_ptr<Widget>(widget));
	widget->m_parent = getWeakPtr();

	if (Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void Widget::removeChild(const std::shared_ptr<Widget>& widget)
{
	FLAT_ASSERT(widget->m_parent.lock() == getWeakPtr().lock());
	std::vector<std::shared_ptr<Widget>>::iterator it = std::find(m_children.begin(), m_children.end(), widget);
	FLAT_ASSERT(it != m_children.end());
	m_children.erase(it);
	widget->m_parent.reset();

	if (Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void Widget::removeFromParent()
{
	FLAT_ASSERT(!m_parent.expired());
	if (RootWidget* root = getRootIfAncestor())
	{
		root->removeDirtyWidget(getWeakPtr());
	}
	m_parent.lock()->removeChild(getSharedPtr());
}

void Widget::removeAllChildren()
{
	for (const std::shared_ptr<Widget>& child : m_children)
	{
		child->m_parent.reset();
	}
	m_children.clear();

	if (Widget* fixedLayoutAncestor = getFixedLayoutAncestor())
		fixedLayoutAncestor->setDirty();
}

void Widget::draw(const render::RenderSettings& renderSettings) const
{
	if (!m_visible)
		return;
		
	const video::Texture* background = m_background.get();
	if (background != nullptr || m_backgroundColor.a > 0.f)
	{
		if (background != nullptr)
		{
			renderSettings.textureUniform.set(background);
			renderSettings.textureGivenUniform.set(true);
		}
		else
		{
			renderSettings.textureGivenUniform.set(false);
		}
		renderSettings.modelMatrixUniform.set(m_transform);

		renderSettings.colorUniform.set(m_backgroundColor);

		if (!m_mouseOver || !click.on())
			renderSettings.secondaryColorUniform.set(video::Color::BLACK);
		else
			renderSettings.secondaryColorUniform.set(video::Color::WHITE);

		// enable vertex attrib array
		// position
		Vector2 size;

		const float position[] = {
			0.f, 0.f,
			m_computedSize.x, 0.f,
			m_computedSize.x, m_computedSize.y,
			0.f, m_computedSize.y
		};

		glEnableVertexAttribArray(renderSettings.positionAttribute);
		glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, position);

		if (background != nullptr)
		{
			// uv
			const float* uv = nullptr;

			if (m_backgroundRepeat == BackgroundRepeat::REPEAT)
			{
				// TODO compute during layout...
				const Vector2& backgroundSize = background->getSize();
				const float uvx0 = m_backgroundPosition.x;
				const float uvx1 = m_backgroundPosition.x + m_computedSize.x / backgroundSize.x;
				const float uvy0 = m_backgroundPosition.y;
				const float uvy1 = m_backgroundPosition.y + m_computedSize.y / backgroundSize.y;
				const float repeatUv[] = {
					uvx0, uvy1,
					uvx1, uvy1,
					uvx1, uvy0,
					uvx0, uvy0
				};

				uv = repeatUv;
			}
			else if (m_backgroundRepeat == BackgroundRepeat::SCALED)
			{
				static const float scaledUv[] = {
					0.0f, 1.0f,
					1.0f, 1.0f,
					1.0f, 0.0f,
					0.0f, 0.0f
				};
				uv = scaledUv;
			}
			else
			{
				FLAT_ASSERT(false);
			}

			glEnableVertexAttribArray(renderSettings.uvAttribute);
			glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, uv);
		}

		// draw
		glDrawArrays(GL_QUADS, 0, 4);

		// disable vertex attrib array
		glDisableVertexAttribArray(renderSettings.positionAttribute);
		if (background != nullptr)
		{
			glDisableVertexAttribArray(renderSettings.uvAttribute);
		}
	}

	drawChildren(renderSettings);
}

bool Widget::isInside(const Vector2& point) const
{
	if (!m_visible)
		return false;

	Vector2 localPosition = getRelativePosition(point);
	return localPosition.x >= 0 && localPosition.x <= m_computedSize.x
	    && localPosition.y >= 0 && localPosition.y <= m_computedSize.y;
}

Vector2 Widget::getRelativePosition(const Vector2& absolutePosition) const
{
	Matrix4 invTransform = inverse(m_transform);
	return Vector2(invTransform * Vector4(absolutePosition, 0.f, 1.f));
}

void Widget::drawChildren(const render::RenderSettings& renderSettings) const
{
	for (const std::shared_ptr<Widget>& child : m_children)
	{
		child->draw(renderSettings);
	}
}

Widget* Widget::getMouseOverWidget(const Vector2& mousePosition)
{
	if (isInside(mousePosition))
	{
		// iterate in reverse order to find top most widgets first
		const std::vector<std::shared_ptr<Widget>>::reverse_iterator end = m_children.rend();
		for (std::vector<std::shared_ptr<Widget>>::reverse_iterator it = m_children.rbegin(); it != end; ++it)
		{
			Widget* child = it->get();
			FLAT_ASSERT(child != nullptr);
			if (Widget* overWidget = child->getMouseOverWidget(mousePosition))
				return overWidget;
		}
		return this;
	}
	return nullptr;
}

void Widget::setDirty()
{
	if (RootWidget* rootWidget = getRootIfAncestor())
	{
		rootWidget->addDirtyWidget(getWeakPtr());
	}
}

void Widget::clearDirty()
{
	if (RootWidget* rootWidget = getRootIfAncestor())
	{
		rootWidget->removeDirtyWidget(getWeakPtr());
	}
}

RootWidget* Widget::getRootIfAncestor()
{
	std::weak_ptr<Widget> widget = getWeakPtr();
	while (!widget.expired() && !widget.lock()->m_parent.expired())
	{
		widget = widget.lock()->m_parent;
	}
	FLAT_ASSERT(!widget.expired());
	Widget* w = widget.lock().get();
	return w->isRoot() ? w->asP<RootWidget>() : nullptr;
}

Widget* Widget::getFixedLayoutAncestor()
{
	if (m_parent.expired())
		return nullptr;

	Widget* parent = m_parent.lock().get();

	if (parent->hasLayout<FixedLayout>())
		return parent;

	if (parent->isRoot() || isRoot())
		return this;

	return parent->getFixedLayoutAncestor();
}

} // ui
} // sharp
} // flat



