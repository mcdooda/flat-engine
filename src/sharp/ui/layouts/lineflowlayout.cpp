#include "lineflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void LineFlowLayout::preLayout(Widget& widget)
{
	Widget::SizePolicy sizePolicy = getSizePolicy(widget);

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
			getComputedSize(widget).y = getComputedSize(parent).y - getMargin(widget).bottom - getMargin(widget).top;
		}
	}

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
			computeExpandWidth(widget);
		}
	}

	if ((sizePolicy & Widget::SizePolicy::COMPRESS) == 0)
	{
		computeTransform(widget);
	}
}

void LineFlowLayout::layout(Widget& widget, bool computePosition)
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

	float totalFixedWidth = getPadding(widget).left + getPadding(widget).right;
	int numFlexibleWidgets = 0;
	for (const std::shared_ptr<Widget>& child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);
		const bool childFixedX = (childSizePolicy & Widget::SizePolicy::FIXED_X) != 0;
		const bool childFixedY = (childSizePolicy & Widget::SizePolicy::FIXED_Y) != 0;
		const bool childCompressX = (childSizePolicy & Widget::SizePolicy::COMPRESS_X) != 0;
		const bool childCompressY = (childSizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0;
		const bool childExpandX = (childSizePolicy & Widget::SizePolicy::EXPAND_X) != 0;

		if (childCompressX || childCompressY)
		{
			child->layout(false);
		}

		// width
		float outerHeight = 0.f;
		if (childFixedY)
		{
			computeFixedHeight(*child);
			outerHeight = getOuterHeight(*child);
		}
		else if (childCompressY)
		{
			outerHeight = getOuterHeight(*child);
		}

		if (compressedSizeY)
		{
			float height = getPadding(widget).top + getPadding(widget).bottom + outerHeight;
			if (size.y < height)
			{
				size.y = height;
			}
		}

		// height
		float outerWidth = 0.f;
		if (childFixedX)
		{
			computeFixedWidth(*child);
			outerWidth = getOuterWidth(*child);
			totalFixedWidth += outerWidth;
		}
		else if (childCompressX)
		{
			outerWidth = getOuterWidth(*child);
			totalFixedWidth += outerWidth;
		}
		else if (childExpandX)
		{
			++numFlexibleWidgets;
		}

		if (compressedSizeX)
		{
			size.x += outerWidth;
		}
	}

	float totalFlexibleWidth = size.x - totalFixedWidth;
	float flexibleWidgetsWidth = numFlexibleWidgets > 0.f ? std::floor(totalFlexibleWidth / numFlexibleWidgets) : 0.f;
	for (const std::shared_ptr<Widget>& child : getChildren(widget))
	{
		Widget::SizePolicy childSizePolicy = getSizePolicy(*child);

		if (childSizePolicy & Widget::SizePolicy::EXPAND_X)
		{
			if (flexibleWidgetsWidth > 0.f)
				getComputedSize(*child).x = flexibleWidgetsWidth - getMargin(*child).left - getMargin(*child).right;
		}

		if (childSizePolicy & Widget::SizePolicy::EXPAND_Y)
		{
			getComputedSize(*child).y = size.y - getPadding(widget).bottom - getPadding(widget).top - getMargin(*child).bottom - getMargin(*child).top;
		}
	}

	if (computePosition && (compressedSizeX || compressedSizeY))
	{
		computeTransform(widget);
	}

	// compute children position
	std::vector<std::shared_ptr<Widget>>& children = getChildren(widget);
	std::vector<std::shared_ptr<Widget>>::iterator it = children.begin();
	std::vector<std::shared_ptr<Widget>>::iterator end = children.end();
	float currentX = getPadding(widget).left;
	for (; it != end; ++it)
	{
		Widget& child = **it;
		currentX += getMargin(child).left;

		Matrix4& childTransform = getTransform(child);
		childTransform = Matrix4();
		float y = 0.f;
		Widget::PositionPolicy positionPolicy = getPositionPolicy(child);
		if ((positionPolicy & Widget::PositionPolicy::BOTTOM) != 0)
		{
			y = getPadding(widget).bottom + getMargin(child).bottom + getPosition(child).y;
		}
		else if ((positionPolicy & Widget::PositionPolicy::TOP) != 0)
		{
			y = getComputedSize(widget).y - getComputedSize(child).y - getPadding(widget).top - getMargin(child).top + getPosition(child).y;
		}
		else
		{
			FLAT_ASSERT((positionPolicy & Widget::PositionPolicy::CENTER_Y) != 0);
			y = (getPadding(widget).bottom + getMargin(child).bottom + getPosition(child).y
				+ getComputedSize(widget).y - getComputedSize(child).y - getPadding(widget).top - getMargin(child).top + getPosition(child).y) / 2.f;
		}
		translateBy(childTransform, { currentX + getPosition(child).x, y });
		transformBy(childTransform, getTransform(widget));

		child.layout(false);

		currentX += getComputedSize(child).x + getMargin(child).right;
	}
}

void LineFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat


