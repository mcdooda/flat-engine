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

Matrix4& Layout::getTransform(Widget& widget)
{
	return widget.m_transform;
}

Widget& Layout::getParent(Widget& widget)
{
	FLAT_ASSERT(widget.m_parent != nullptr);
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

Widget::SizePolicy & Layout::getSizePolicy(Widget & widget)
{
	return widget.m_sizePolicy;
}

Widget::Size& Layout::getSize(Widget& widget)
{
	return widget.m_size;
}

Widget::Size& Layout::getComputedSize(Widget& widget)
{
	return widget.m_computedSize;
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
	return widget.m_position.x + widget.m_computedSize.x + widget.m_margin.right;
}

float Layout::getParentRelativeBottom(Widget& widget)
{
	return widget.m_position.y + widget.m_computedSize.y + widget.m_margin.bottom;
}

void Layout::computeWidth(Widget & widget)
{
	Widget::SizePolicy sizePolicy = widget.m_sizePolicy;

	if (sizePolicy & Widget::SizePolicy::FIXED_X)
	{
		widget.m_computedSize.x = widget.m_size.x;
	}
	else if (sizePolicy & Widget::SizePolicy::COMPRESS_X)
	{
		FLAT_ASSERT_MSG(false, "not implemented");
	}
	else if (sizePolicy & Widget::SizePolicy::EXPAND_X)
	{
		// already computed by the parent's layout
	}
	else
	{
		FLAT_ASSERT(false);
	}
}

void Layout::computeHeight(Widget & widget)
{
	Widget::SizePolicy sizePolicy = widget.m_sizePolicy;

	if (sizePolicy & Widget::SizePolicy::FIXED_Y)
	{
		widget.m_computedSize.y = widget.m_size.y;
	}
	else if (sizePolicy & Widget::SizePolicy::COMPRESS_Y)
	{
		FLAT_ASSERT_MSG(false, "not implemented");
	}
	else if (sizePolicy & Widget::SizePolicy::EXPAND_Y)
	{
		// already computed by the parent's layout
	}
	else
	{
		FLAT_ASSERT(false);
	}
}

void Layout::computePosition(Widget& widget, Vector2& position)
{
	Widget::PositionPolicy positionPolicy = widget.m_positionPolicy;

	FLAT_ASSERT(widget.m_parent != nullptr);
	Widget& parent = *widget.m_parent;

	if (positionPolicy & Widget::PositionPolicy::LEFT)
	{
		position.x = parent.m_padding.left + widget.m_margin.left;
	}
	else if (positionPolicy & Widget::PositionPolicy::RIGHT)
	{
		position.x = parent.m_computedSize.x - parent.m_padding.right
				- widget.m_computedSize.x - widget.m_margin.right;
	}
	else if (positionPolicy & Widget::PositionPolicy::CENTER_X)
	{
		position.x = (parent.m_padding.left + parent.m_computedSize.x - parent.m_padding.right) / 2.f
				- getOuterWidth(widget) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}

	if (positionPolicy & Widget::PositionPolicy::TOP)
	{
		position.y = parent.m_computedSize.y - parent.m_padding.top
				- widget.m_computedSize.y - widget.m_margin.top;
	}
	else if (positionPolicy & Widget::PositionPolicy::BOTTOM)
	{
		position.y = parent.m_padding.bottom + widget.m_margin.bottom;
	}
	else if (positionPolicy & Widget::PositionPolicy::CENTER_Y)
	{
		position.y = (parent.m_padding.bottom + parent.m_computedSize.y - parent.m_padding.top) / 2.f
				- getOuterHeight(widget) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}

	position += widget.m_position;
}

void Layout::computeTransform(Widget& widget)
{
	Vector2 position;
	computePosition(widget, position);

	Matrix4& transform = widget.m_transform;
	transform = widget.m_parent->m_transform;
	translateBy(transform, position + widget.m_computedSize / 2.f);
	//transform.rotateZ(widget.getRotation().z);
	//transform.rotateY(widget.getRotation().y);
	//transform.rotateX(widget.getRotation().x);
	translateBy(transform, -widget.m_computedSize / 2.f);
}

} // ui
} // sharp
} // flat


