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
		computeWidth(widget);
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
		computeHeight(widget);
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

		// width
		float outerWidth = 0.f;
		if (childSizePolicy & Widget::SizePolicy::FIXED_X)
		{
			computeWidth(*child);
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
		if (childSizePolicy & Widget::SizePolicy::FIXED_Y)
		{
			computeHeight(*child);
			outerHeight = getOuterHeight(*child);
			totalFixedHeight += outerHeight;
		}
		else if (childSizePolicy & Widget::SizePolicy::EXPAND_Y)
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


