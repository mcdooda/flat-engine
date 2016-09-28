#include "columnflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void ColumnFlowLayout::preLayout(Widget& widget)
{
	childrenPreLayout(widget);
}

void ColumnFlowLayout::layout(Widget& widget)
{
	// compute size
	Vector2 size(getPadding(widget).left + getPadding(widget).right, getPadding(widget).top + getPadding(widget).bottom);
	for (Widget* child : getChildren(widget))
	{
		size.y += getOuterHeight(*child);

		float width = getPadding(widget).left + getPadding(widget).right + getOuterWidth(*child);
		if (size.x < width)
		{
			size.x = width;
		}
	}

	getSize(widget) = size;

	computeTransform(widget);

	// compute children position (reverse order)
	std::vector<Widget*>& children = getChildren(widget);
	std::vector<Widget*>::reverse_iterator it = children.rbegin();
	std::vector<Widget*>::reverse_iterator end = children.rend();
	float currentY = getPadding(**it).bottom;
	for (; it != end; ++it)
	{
		Widget& child = **it;
		currentY += getMargin(child).bottom;

		Matrix4& childTransform = getTransform(child);
		childTransform = Matrix4();
		translateBy(childTransform, Vector2(getPadding(widget).left + getMargin(child).left + getPosition(child).x, currentY + getPosition(child).y));
		transformBy(childTransform, getTransform(widget));

		childrenLayout(child);

		currentY += getSize(child).y + getMargin(child).top;
	}
}

void ColumnFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat


