#include <iostream>
#include "columnflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void ColumnFlowLayout::preLayout(Widget& widget)
{
	Widget::SizePolicy sizePolicy = getSizePolicy(widget);

	if (sizePolicy & Widget::SizePolicy::FIXED_X)
	{
		computeFixedWidth(widget);
	}
	else if (sizePolicy & Widget::SizePolicy::EXPAND_X)
	{
		Widget& parent = getParent(widget);
		Widget::SizePolicy parentSizePolicy = getSizePolicy(parent);
		if (parentSizePolicy & Widget::SizePolicy::FIXED_X)
		{
			getComputedSize(widget).x = getComputedSize(parent).x - getMargin(widget).left - getMargin(widget).right;
		}
	}

	if (sizePolicy & Widget::SizePolicy::FIXED_Y)
	{
		computeFixedHeight(widget);
	}
	else if (sizePolicy & Widget::SizePolicy::EXPAND_Y)
	{
		Widget& parent = getParent(widget);
		Widget::SizePolicy parentSizePolicy = getSizePolicy(parent);
		if (parentSizePolicy & Widget::SizePolicy::FIXED_Y)
		{
			computeExpandHeight(widget);
		}
	}

	if ((sizePolicy & Widget::SizePolicy::COMPRESS) == 0)
	{
		computeTransform(widget);
	}
}

void ColumnFlowLayout::layout(Widget& widget, bool computePosition)
{
	childrenPreLayout(widget);

	// compute children size and widget compressed size
	Widget::SizePolicy sizePolicy = getSizePolicy(widget);
	bool compressedSizeX = (sizePolicy & Widget::SizePolicy::COMPRESS_X) != 0;
	bool compressedSizeY = (sizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0;

	Vector2& size = getComputedSize(widget);
	if (compressedSizeX)
	{
		size.x = getPadding(widget).left + getPadding(widget).right;
	}
	if (compressedSizeY)
	{
		size.y = getPadding(widget).bottom + getPadding(widget).top;
	}

	float totalFixedHeight = getPadding(widget).bottom + getPadding(widget).top;
	int numFlexibleWidgets = 0;
	for (const std::shared_ptr<Widget>& child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);
		const bool childFixedX    = (childSizePolicy & Widget::SizePolicy::FIXED_X) != 0;
		const bool childFixedY    = (childSizePolicy & Widget::SizePolicy::FIXED_Y) != 0;
		const bool childCompressX = (childSizePolicy & Widget::SizePolicy::COMPRESS_X) != 0;
		const bool childCompressY = (childSizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0;
		const bool childExpandY   = (childSizePolicy & Widget::SizePolicy::EXPAND_Y) != 0;

		if (childCompressX || childCompressY)
		{
			child->layout(false);
		}

		// width
		float outerWidth = 0.f;
		if (childFixedX)
		{
			computeFixedWidth(*child);
			outerWidth = getOuterWidth(*child);
		}
		else if (childCompressX)
		{
			outerWidth = getOuterWidth(*child);
		}

		if (compressedSizeX)
		{
			float width = getPadding(widget).left + getPadding(widget).right + outerWidth;
			if (size.x < width)
			{
				size.x = width;
			}
		}

		// height
		float outerHeight = 0.f;
		if (childFixedY)
		{
			computeFixedHeight(*child);
			outerHeight = getOuterHeight(*child);
			totalFixedHeight += outerHeight;
		}
		else if (childCompressY)
		{
			outerHeight = getOuterHeight(*child);
			totalFixedHeight += outerHeight;
		}
		else if (childExpandY)
		{
			++numFlexibleWidgets;
		}

		if (compressedSizeY)
		{
			size.y += outerHeight;
		}
	}

	float totalFlexibleHeight = size.y - totalFixedHeight;
	float flexibleWidgetsHeight = numFlexibleWidgets > 0.f ? std::floor(totalFlexibleHeight / numFlexibleWidgets) : 0.f;
	for (const std::shared_ptr<Widget>& child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);

		if (childSizePolicy & Widget::SizePolicy::EXPAND_X)
		{
			getComputedSize(*child).x = size.x - getPadding(widget).left - getPadding(widget).right - getMargin(*child).left - getMargin(*child).bottom;
		}

		if (childSizePolicy & Widget::SizePolicy::EXPAND_Y)
		{
			if (flexibleWidgetsHeight > 0.f)
				getComputedSize(*child).y = flexibleWidgetsHeight - getMargin(*child).bottom - getMargin(*child).top;
		}
	}

	if (computePosition && (compressedSizeX || compressedSizeY))
	{
		computeTransform(widget);
	}

	// compute children position
	Widget::ScrollPosition& scrollPosition = getScrollPosition(widget);
	std::vector<std::shared_ptr<Widget>>& children = getChildren(widget);
	std::vector<std::shared_ptr<Widget>>::iterator it = children.begin();
	std::vector<std::shared_ptr<Widget>>::iterator end = children.end();
	float currentY = size.y - getPadding(widget).top;
	for (; it != end; ++it)
	{
		Widget& child = **it;
		currentY -= getMargin(child).top + getComputedSize(child).y;

		Matrix4& childTransform = getTransform(child);
		childTransform = Matrix4();
		float x = 0.f;
		Widget::PositionPolicy positionPolicy = getPositionPolicy(child);
		if ((positionPolicy & Widget::PositionPolicy::LEFT) != 0)
		{
			x = getPadding(widget).left + getMargin(child).left + getPosition(child).x;
		}
		else if ((positionPolicy & Widget::PositionPolicy::RIGHT) != 0)
		{
			x = getComputedSize(widget).x - getComputedSize(child).x - getPadding(widget).right - getMargin(child).right + getPosition(child).x;
		}
		else
		{
			FLAT_ASSERT((positionPolicy & Widget::PositionPolicy::CENTER_X) != 0);
			x = (getPadding(widget).left + getMargin(child).left + getPosition(child).x
				+ getComputedSize(widget).x - getComputedSize(child).x - getPadding(widget).right - getMargin(child).right + getPosition(child).x) / 2.f;
		}
		translateBy(
			childTransform,
			{
				std::round(x - scrollPosition.x),
				std::round(currentY + getPosition(child).y - scrollPosition.y)
			}
		);
		transformBy(childTransform, getTransform(widget));

		child.layout(false);

		currentY -= getMargin(child).bottom;
	}

	Widget::ScrollPosition& minScrollPosition = getMinScrollPosition(widget);
	minScrollPosition.x = 0.f;
	minScrollPosition.y = currentY;

	scrollPosition.y = std::max(scrollPosition.y, minScrollPosition.y);
	scrollPosition.y = std::min(scrollPosition.y, 0.f);
}

void ColumnFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat


