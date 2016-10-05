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
			getComputedSize(widget).x = getComputedSize(parent).x;
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
			getComputedSize(widget).y = getComputedSize(parent).y;
		}
	}

	computeTransform(widget);

	childrenPreLayout(widget);
}

void ColumnFlowLayout::layout(Widget& widget)
{
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
	for (Widget* child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);
		const bool childFixedX    = (childSizePolicy & Widget::SizePolicy::FIXED_X) != 0;
		const bool childFixedY    = (childSizePolicy & Widget::SizePolicy::FIXED_Y) != 0;
		const bool childCompressX = (childSizePolicy & Widget::SizePolicy::COMPRESS_X) != 0;
		const bool childCompressY = (childSizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0;
		const bool childExpandY   = (childSizePolicy & Widget::SizePolicy::EXPAND_Y) != 0;

		if (childCompressX || childCompressY)
		{
			child->layout();
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
	float flexibleWidgetsHeight = std::floor(totalFlexibleHeight / numFlexibleWidgets);
	for (Widget* child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);

		if (childSizePolicy & Widget::SizePolicy::EXPAND_X)
		{
			getComputedSize(*child).x = size.x - getPadding(widget).left - getPadding(widget).right;
		}

		if (childSizePolicy & Widget::SizePolicy::EXPAND_Y)
		{
			getComputedSize(*child).y = flexibleWidgetsHeight;
		}
	}

	// compute children position (reverse order)
	std::vector<Widget*>& children = getChildren(widget);
	std::vector<Widget*>::iterator it = children.begin();
	std::vector<Widget*>::iterator end = children.end();
	float currentY = size.y - getPadding(widget).top;
	for (; it != end; ++it)
	{
		Widget& child = **it;
		currentY -= getMargin(child).top + getComputedSize(child).y;

		Matrix4& childTransform = getTransform(child);
		childTransform = Matrix4();
		translateBy(childTransform, Vector2(getPadding(widget).left + getMargin(child).left + getPosition(child).x, currentY + getPosition(child).y));
		transformBy(childTransform, getTransform(widget));

		child.layout();

		currentY -= getMargin(child).bottom;
	}
}

void ColumnFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat


