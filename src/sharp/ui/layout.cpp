#include "layout.h"
#include "widget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void Layout::childrenPreLayout(Widget& widget)
{
	for (Widget* child : widget.m_children)
	{
		child->preLayout();
	}
}

void Layout::childrenLayout(Widget& widget)
{
	for (Widget* child : widget.m_children)
	{
		child->layout();
	}
}

void Layout::childrenPostLayout(Widget& widget)
{
	for (Widget* child : widget.m_children)
	{
		child->postLayout();
	}
}

void Layout::childrenFullLayout(Widget& widget)
{
	for (Widget* child : widget.m_children)
	{
		child->fullLayout();
	}
}

std::vector<Widget*>& Layout::getChildren(Widget& widget)
{
	return widget.m_children;
}

geometry::Matrix4& Layout::getTransform(Widget& widget)
{
	return widget.m_transform;
}

Widget& Layout::getParent(Widget& widget)
{
	FLAT_ASSERT(widget.m_parent);
	return *widget.m_parent;
}

Widget::PositionPolicy& Layout::getPositionPolicy(Widget& widget)
{
	return widget.m_positionPolicy;
}

Widget::Position& Layout::getPosition(Widget& widget)
{
	return widget.m_position;
}

Widget::Size& Layout::getSize(Widget& widget)
{
	return widget.m_size;
}

Widget::Margin& Layout::getMargin(Widget& widget)
{
	return widget.m_margin;
}

Widget::Padding& Layout::getPadding(Widget& widget)
{
	return widget.m_padding;
}

float Layout::getOuterWidth(Widget& widget)
{
	return widget.getOuterWidth();
}

float Layout::getOuterHeight(Widget& widget)
{
	return widget.getOuterHeight();
}

float Layout::getParentRelativeRight(Widget& widget)
{
	return widget.m_position.x + widget.m_size.x + widget.m_margin.right;
}

float Layout::getParentRelativeBottom(Widget& widget)
{
	return widget.m_position.y + widget.m_size.y + widget.m_margin.bottom;
}

void Layout::computePosition(Widget& widget, geometry::Vector2& position)
{
	Widget::PositionPolicy positionPolicy = widget.m_positionPolicy;

	FLAT_ASSERT(widget.m_parent);
	Widget& parent = *widget.m_parent;

	if (positionPolicy & Widget::PositionPolicy::LEFT)
	{
		position.x = parent.m_padding.left + widget.m_margin.left + widget.m_position.x;
	}
	else if (positionPolicy & Widget::PositionPolicy::RIGHT)
	{
		position.x = parent.m_size.x - parent.m_padding.right
				- widget.m_size.x - widget.m_margin.right;
	}
	else if (positionPolicy & Widget::PositionPolicy::CENTER_X)
	{
		position.x = (parent.m_padding.left + parent.m_size.x - parent.m_padding.right) / 2.f
				- getOuterWidth(widget) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}

	if (positionPolicy & Widget::PositionPolicy::TOP)
	{
		position.y = parent.m_padding.top + widget.m_margin.top + widget.m_position.y;
	}
	else if (positionPolicy & Widget::PositionPolicy::BOTTOM)
	{
		position.y = parent.m_size.y - parent.m_padding.bottom
				- widget.m_size.y - widget.m_margin.bottom;
	}
	else if (positionPolicy & Widget::PositionPolicy::CENTER_Y)
	{
		position.y = (parent.m_padding.top + parent.m_size.y - parent.m_padding.bottom) / 2.f
				- getOuterHeight(widget) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}
}

void Layout::computeTransform(Widget& widget)
{
	geometry::Vector2 position;
	computePosition(widget, position);

	geometry::Matrix4& transform = widget.m_transform;
	transform.setIdentity();
	transform.translate(-widget.getSize() / 2.f);
	transform.rotateZ(widget.getRotation().z);
	transform.rotateY(widget.getRotation().y);
	transform.rotateX(widget.getRotation().x);
	transform.translate(position + widget.getSize() / 2.f);
	transform = widget.m_parent->m_transform * transform;
}

} // ui
} // sharp
} // flat


