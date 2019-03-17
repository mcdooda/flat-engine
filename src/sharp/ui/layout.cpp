#include "sharp/ui/layout.h"
#include "sharp/ui/widget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void Layout::childrenPreLayout(Widget& widget)
{
	for (const std::shared_ptr<Widget>& child : widget.m_children)
	{
		child->preLayout();
	}
}

void Layout::childrenLayout(Widget& widget)
{
	for (const std::shared_ptr<Widget>& child : widget.m_children)
	{
		child->layout(true);
	}
}

void Layout::childrenPostLayout(Widget& widget)
{
	for (const std::shared_ptr<Widget>& child : widget.m_children)
	{
		child->postLayout();
	}
}

void Layout::childrenFullLayout(Widget& widget)
{
	for (const std::shared_ptr<Widget>& child : widget.m_children)
	{
		child->fullLayout();
	}
}

std::vector<std::shared_ptr<Widget>>& Layout::getChildren(Widget& widget)
{
	return widget.m_children;
}

Matrix4& Layout::getTransform(Widget& widget)
{
	return widget.m_transform;
}

Widget& Layout::getParent(Widget& widget)
{
	FLAT_ASSERT(!widget.m_parent.expired());
	return *widget.m_parent.lock().get();
}

Widget::PositionPolicy& Layout::getPositionPolicy(Widget& widget)
{
	return widget.m_positionPolicy;
}

Widget::Position& Layout::getPosition(Widget& widget)
{
	return widget.m_position;
}

Widget::SizePolicy& Layout::getSizePolicy(Widget& widget)
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

Widget::ScrollPosition& Layout::getScrollPosition(Widget& widget)
{
	return widget.m_scrollPosition;
}

Widget::ScrollPosition& Layout::getMinScrollPosition(Widget & widget)
{
	return widget.m_minScrollPosition;
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

void Layout::computeFixedWidth(Widget& widget)
{
	FLAT_ASSERT((widget.m_sizePolicy & Widget::SizePolicy::FIXED_X) != 0);
	widget.m_computedSize.x = widget.m_size.x;
}

void Layout::computeFixedHeight(Widget& widget)
{
	FLAT_ASSERT((widget.m_sizePolicy & Widget::SizePolicy::FIXED_Y) != 0);
	widget.m_computedSize.y = widget.m_size.y;
}

void Layout::computeExpandWidth(Widget& widget)
{
	FLAT_ASSERT((widget.m_sizePolicy & Widget::SizePolicy::EXPAND_X) != 0);
	FLAT_ASSERT(!widget.m_parent.expired());
	Widget& parent = *widget.m_parent.lock();
	widget.m_computedSize.x = parent.m_computedSize.x - widget.m_margin.left - widget.m_margin.right;
}

void Layout::computeExpandHeight(Widget& widget)
{
	FLAT_ASSERT((widget.m_sizePolicy & Widget::SizePolicy::EXPAND_Y) != 0);
	FLAT_ASSERT(!widget.m_parent.expired());
	Widget& parent = *widget.m_parent.lock();
	widget.m_computedSize.y = parent.m_computedSize.y - widget.m_margin.bottom - widget.m_margin.top;
}

void Layout::computeCompressWidth(Widget& widget)
{
	int numExpandChildren = 0;
	float compressWidth = 0;
	for (const std::shared_ptr<Widget>& childSharedPtr : widget.m_children)
	{
		Widget* child = childSharedPtr.get();
		const Widget::SizePolicy sizePolicy = child->getSizePolicy();
		if ((sizePolicy & Widget::SizePolicy::FIXED_X) != 0)
		{
			compressWidth = std::max(compressWidth, child->m_computedSize.x + child->m_margin.left + child->m_margin.right);
		}
		else if ((sizePolicy & Widget::SizePolicy::COMPRESS_X) != 0)
		{
			//computeCompressWidth(*child);
			child->fullLayout();
			compressWidth = std::max(compressWidth, child->m_computedSize.x + child->m_margin.left + child->m_margin.right);
		}
		else if ((sizePolicy & Widget::SizePolicy::EXPAND_X) != 0)
		{
			++numExpandChildren;
			continue;
		}
		else
		{
			FLAT_ASSERT(false);
		}
	}
	compressWidth += widget.m_padding.left + widget.m_padding.right;
	widget.m_computedSize.x = compressWidth;

	if (numExpandChildren > 0)
	{
		FLAT_ASSERT_MSG(numExpandChildren < widget.m_children.size(), "There must be at least 1 non expand widget inside a compress widget");
		for (const std::shared_ptr<Widget>& childSharedPtr : widget.m_children)
		{
			Widget* child = childSharedPtr.get();
			const Widget::SizePolicy sizePolicy = child->getSizePolicy();
			if ((sizePolicy & Widget::SizePolicy::EXPAND_X) != 0)
			{
				computeExpandWidth(*child);
			}
		}
	}
}

void Layout::computeCompressHeight(Widget& widget)
{
	int numExpandChildren = 0;
	float compressHeight = 0;
	for (const std::shared_ptr<Widget>& childSharedPtr : widget.m_children)
	{
		Widget* child = childSharedPtr.get();
		const Widget::SizePolicy sizePolicy = child->getSizePolicy();
		if ((sizePolicy & Widget::SizePolicy::FIXED_Y) != 0)
		{
			compressHeight = std::max(compressHeight, child->m_computedSize.y + child->m_margin.bottom + child->m_margin.top);
		}
		else if ((sizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0)
		{
			//computeCompressHeight(*child);
			child->fullLayout();
			compressHeight = std::max(compressHeight, child->m_computedSize.y + child->m_margin.bottom + child->m_margin.top);
		}
		else if ((sizePolicy & Widget::SizePolicy::EXPAND_Y) != 0)
		{
			++numExpandChildren;
			continue;
		}
		else
		{
			FLAT_ASSERT(false);
		}
	}
	compressHeight += widget.m_padding.bottom + widget.m_padding.top;
	widget.m_computedSize.y = compressHeight;

	if (numExpandChildren > 0)
	{
		FLAT_ASSERT_MSG(numExpandChildren < widget.m_children.size(), "There must be at least 1 non expand widget inside a compress widget");
		for (const std::shared_ptr<Widget>& childSharedPtr : widget.m_children)
		{
			Widget* child = childSharedPtr.get();
			const Widget::SizePolicy sizePolicy = child->getSizePolicy();
			if ((sizePolicy & Widget::SizePolicy::EXPAND_Y) != 0)
			{
				computeExpandHeight(*child);
			}
		}
	}
}

void Layout::computePosition(Widget& widget, Vector2& position)
{
	computePositionX(widget, position);
	computePositionY(widget, position);
}

void Layout::computePositionX(Widget& widget, Vector2& position)
{
	Widget::PositionPolicy positionPolicy = widget.m_positionPolicy;
	Widget& parent = getParent(widget);

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
			- (widget.m_computedSize.x + widget.m_margin.left - widget.m_margin.right) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}

	position.x += widget.m_position.x;
}

void Layout::computePositionY(Widget& widget, Vector2& position)
{
	Widget::PositionPolicy positionPolicy = widget.m_positionPolicy;
	Widget& parent = getParent(widget);

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
			- (widget.m_computedSize.y + widget.m_margin.bottom - widget.m_margin.top) / 2.f;
	}
	else
	{
		FLAT_ASSERT(false);
	}

	position.y += widget.m_position.y;
}

void Layout::computeTransform(Widget& widget)
{
	Vector2 position;
	computePosition(widget, position);

	Widget& parent = getParent(widget);
	const Widget::ScrollPosition& parentScrollPosition = getScrollPosition(parent);

	Matrix4& transform = widget.m_transform;
	transform = parent.m_transform;
	translateBy(
		transform,
		{
			std::roundf(position.x - parentScrollPosition.x),
			std::roundf(position.y - parentScrollPosition.y)
		}
	);

	//translateBy(transform, position + widget.m_computedSize / 2.f);
	//transform.rotateZ(widget.getRotation().z);
	//transform.rotateY(widget.getRotation().y);
	//transform.rotateX(widget.getRotation().x);
	//translateBy(transform, -widget.m_computedSize / 2.f);
}

} // ui
} // sharp
} // flat


