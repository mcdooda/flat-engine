#include <iostream>
#include <limits>
#include <algorithm>
#include "widget.h"
#include "rootwidget.h"
#include "layouts/fixedlayout.h"
#include "../../render/rendersettings.h"
#include "../../video/color.h"
#include "../../video/texture.h"
#include "../../memory/memory.h"
#include "../../debug/helpers.h"

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
	m_backgroundSize(0.f, 0.f),
	m_backgroundRepeat(BackgroundRepeat::SCALED),
	m_backgroundColor(0.f, 0.f, 0.f, 0.f),
	m_sizePolicy(SizePolicy::FIXED),
	m_positionPolicy(PositionPolicy::TOP_LEFT),
	m_visible(true),
	m_allowScrollX(false),
	m_allowScrollY(false),
	m_allowDragScrolling(false),
	m_restrictScrollX(true),
	m_restrictScrollY(true),
	m_mouseOver(false),
	m_hasFocus(false),
	m_scrolled(false),
	m_dragged(false)
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
	setAncestorDirty();
}

void Widget::setPositionPolicy(PositionPolicy positionPolicy)
{
	FLAT_ASSERT(!((positionPolicy & PositionPolicy::FLOW_X) && (positionPolicy & PositionPolicy::FLOW_Y)));
	m_positionPolicy = positionPolicy;
}

void Widget::setPosition(const Position& position)
{
	m_position = position;
	setAncestorDirty();
}

void Widget::setAbsolutePosition(const Vector2& absolutePosition)
{
	m_position += absolutePosition - getAbsolutePosition();
	setAncestorDirty();
}

Vector2 Widget::getAbsolutePosition() const
{
	return Vector2(m_transform[3][0], m_transform[3][1]);
}

void Widget::setBackground(const std::shared_ptr<const video::Texture>& background)
{
	m_background = background;
	m_backgroundSize = background->getSize();
	setBackgroundColor(flat::video::Color::WHITE);
}

void Widget::setAllowScrollX(bool allowScrollX)
{
	if(!m_allowScrollY)
	{
		if (allowScrollX && !m_allowScrollX)
		{
			mouseWheelMove.on(this, &Widget::mouseWheelMoved);
		}
		else
		{
			mouseWheelMove.off(this);
		}
	}
	m_allowScrollX = allowScrollX; 
}

void Widget::setAllowScrollY(bool allowScrollY)
{
	if(!m_allowScrollX)
	{
		if (allowScrollY && !m_allowScrollY)
		{
			mouseWheelMove.on(this, &Widget::mouseWheelMoved);
		}
		else
		{
			mouseWheelMove.off(this);
		}
	}	
	m_allowScrollY = allowScrollY;
}

bool Widget::mouseWheelMoved(Widget* widget, bool& handled, const Vector2& offset)
{
	handled = true;
	if (offset.x != 0)
	{
		scrollX(offset.x);
	}
	if (offset.y != 0)
	{
		scrollY(offset.y);
	}
	return true;
}

void Widget::scrollX(float scrollValueX)
{
	setScrollX(m_scrollPosition.x + scrollValueX * SCROLL_SPEED);
}

void Widget::scrollY(float scrollValueY)
{
	setScrollY(m_scrollPosition.y + scrollValueY * SCROLL_SPEED);
}

void Widget::setScrollX(float scrollX)
{
	if (m_allowScrollX)
	{
		m_scrollPosition.x = scrollX;

		if (m_restrictScrollX)
		{
			m_scrollPosition.x = std::min(m_scrollPosition.x, 0.f);
			m_scrollPosition.x = std::max(m_scrollPosition.x, m_minScrollPosition.x);
		}

		m_scrolled = true;

		setAncestorDirty();
	}
}

void Widget::setScrollY(float scrollY)
{
	if (m_allowScrollY)
	{
		m_scrollPosition.y = scrollY;

		if (m_restrictScrollY)
		{
			m_scrollPosition.y = std::max(m_scrollPosition.y, m_minScrollPosition.y);
			m_scrollPosition.y = std::min(m_scrollPosition.y, 0.f);
		}

		m_scrolled = true;

		setAncestorDirty();
	}
}

void Widget::setScrollPosition(const ScrollPosition& scrollPosition)
{
	setScrollX(scrollPosition.x);
	setScrollY(scrollPosition.y);
}

void Widget::drag()
{
	RootWidget* root = getRootIfAncestor();
	FLAT_ASSERT(root != nullptr);
	root->drag(this);
}

void Widget::drop()
{
	RootWidget* root = getRootIfAncestor();
	FLAT_ASSERT(root != nullptr);
	root->drop(this);
}

void Widget::addChild(const std::shared_ptr<Widget>& widget)
{
	FLAT_ASSERT_MSG(widget->m_parent.expired(), "Cannot add a node as child if it already has a parent");
	FLAT_ASSERT_MSG(widget.get() != this, "A node cannot add itself as its child");
	m_children.push_back(std::shared_ptr<Widget>(widget));
	widget->m_parent = getWeakPtr();

	setAncestorDirty();
}

void Widget::removeChild(const std::shared_ptr<Widget>& widget)
{
	FLAT_ASSERT(widget->m_parent.lock() == getWeakPtr().lock());
	std::vector<std::shared_ptr<Widget>>::iterator it = std::find(m_children.begin(), m_children.end(), widget);
	FLAT_ASSERT(it != m_children.end());
	m_children.erase(it);
	// set the ancestor dirty before resetting the parent!
	setAncestorDirty();
	widget->m_parent.reset();
}

void Widget::removeFromParent()
{
	FLAT_ASSERT_MSG(!m_parent.expired(), "the widget has not parent");
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

	setAncestorDirty();
}

void Widget::layoutDone()
{
	layoutFinished(this);
}

void Widget::draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	if (!m_visible)
	{
		return;
	}

	ScissorRectangle scissor;
	if (!computeAndApplyScissor(scissor, parentScissor))
	{
		return;
	}
		
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
		renderSettings.vertexColorGivenUniform.set(false);
		
		renderSettings.colorUniform.set(m_backgroundColor);

		renderSettings.secondaryColorUniform.set(video::Color::BLACK);

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
			float uv[8];

			if (m_backgroundRepeat == BackgroundRepeat::REPEAT)
			{
				FLAT_ASSERT(m_backgroundSize.x > 0.f && m_backgroundSize.y > 0.f);
				// TODO compute during layout...
				const float uvx0 = m_backgroundPosition.x;
				const float uvx1 = m_backgroundPosition.x + m_computedSize.x / m_backgroundSize.x;
				const float uvy0 = m_backgroundPosition.y;
				const float uvy1 = m_backgroundPosition.y + m_computedSize.y / m_backgroundSize.y;

				uv[0] = uvx0; uv[1] = uvy1;
				uv[2] = uvx1; uv[3] = uvy1;
				uv[4] = uvx1; uv[5] = uvy0;
				uv[6] = uvx0; uv[7] = uvy0;
			}
			else if (m_backgroundRepeat == BackgroundRepeat::SCALED)
			{
				constexpr float uvx0 = 0.f;
				constexpr float uvx1 = 1.f;
				constexpr float uvy0 = 0.f;
				constexpr float uvy1 = 1.f;

				uv[0] = uvx0; uv[1] = uvy1;
				uv[2] = uvx1; uv[3] = uvy1;
				uv[4] = uvx1; uv[5] = uvy0;
				uv[6] = uvx0; uv[7] = uvy0;
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

	drawChildren(renderSettings, scissor);
}

CursorType Widget::getCursorType() const
{
	if (leftClick.on())
	{
		return CURSOR(HAND);
	}

	if (!m_parent.expired())
	{
		Widget* parent = m_parent.lock().get();
		return parent->getCursorType();
	}

	FLAT_ASSERT_MSG(false, "Cannot get cursor of a node with no parent!");
	return INVALID_CURSOR;
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
	return Vector2(inverse(m_transform) * Vector4(absolutePosition, 0.f, 1.f));
}

Vector2 Widget::getRelativePosition(const Widget& other) const
{
	return Vector2(other.m_transform[3][0] - m_transform[3][0], other.m_transform[3][1] - m_transform[3][1]);
}

bool Widget::intersect(const ScissorRectangle& a, const ScissorRectangle& b)
{
	return a.x < b.x + b.width
	    && b.x < a.x + a.width
	    && a.y < b.y + b.height
	    && b.y < a.y + a.height;
}
void Widget::computeParentScissorIntersection(ScissorRectangle& scissor, const ScissorRectangle& parentScissor)
{
	if (intersect(scissor, parentScissor))
	{
		GLint left = std::max(scissor.x, parentScissor.x);
		GLint right = std::min(scissor.x + scissor.width, parentScissor.x + parentScissor.width);
		GLint bottom = std::max(scissor.y, parentScissor.y);
		GLint top = std::min(scissor.y + scissor.height, parentScissor.y + parentScissor.height);

		scissor.x = left;
		scissor.y = bottom;
		scissor.width = right - left;
		scissor.height = top - bottom;
	}
	else
	{
		scissor.x = 0;
		scissor.y = 0;
		scissor.width = 0;
		scissor.height = 0;
	}
}

void Widget::getScissor(ScissorRectangle& scissor) const
{
	scissor.x = static_cast<GLint>(m_transform[3][0]);
	scissor.y = static_cast<GLint>(m_transform[3][1]);
	scissor.width = static_cast<GLsizei>(m_computedSize.x);
	scissor.height = static_cast<GLsizei>(m_computedSize.y);
}

bool Widget::computeAndApplyScissor(ScissorRectangle& scissor, const ScissorRectangle& parentScissor) const
{
	getScissor(scissor);
	computeParentScissorIntersection(scissor, parentScissor);
	if (scissor.width > 0 && scissor.height > 0)
	{
		glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
		return true;
	}
	return false;
}

void Widget::drawChildren(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	for (const std::shared_ptr<Widget>& child : m_children)
	{
		child->draw(renderSettings, parentScissor);
	}
}

Vector2 Widget::getMaxScrollPosition() const
{
	Vector2 maxScrollPosition;

	for (const std::shared_ptr<Widget>& child : m_children)
	{
		float right = child->m_transform[3][0] + child->m_computedSize.x + child->m_margin.right;
		maxScrollPosition.x = std::max(right, maxScrollPosition.x);
		float bottom = child->m_transform[3][1] - child->m_margin.bottom;
		maxScrollPosition.y = std::min(bottom, maxScrollPosition.y);
	}

	maxScrollPosition -= m_computedSize;

	return maxScrollPosition;
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

bool Widget::canBeFocused() const
{
	return false;
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
	return w->isRoot() ? dynamic_cast<RootWidget*>(w) : nullptr;
}

Widget* Widget::getFixedLayoutAncestor()
{
	if (isRoot())
		return this;
	
	if (m_parent.expired())
		return nullptr;

	Widget* parent = m_parent.lock().get();

	if (parent->hasFixedSize())
		return parent;

	if (hasFixedSize())
		return parent->getFixedLayoutAncestor();

	return parent->getFixedLayoutAncestor();
}

void Widget::setAncestorDirty()
{
	Widget* fixedLayoutAncestor = getFixedLayoutAncestor();
	if (fixedLayoutAncestor != nullptr)
	{
		fixedLayoutAncestor->setDirty();
	}
}

bool Widget::hasFixedSize() const
{
	return hasLayout<FixedLayout>() && m_sizePolicy == Widget::SizePolicy::FIXED;
}

} // ui
} // sharp
} // flat



