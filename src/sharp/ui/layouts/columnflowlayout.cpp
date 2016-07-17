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
	geometry::Vector2 size(getPadding(widget).left + getPadding(widget).right, getPadding(widget).top + getPadding(widget).bottom);
	for (Widget* child : getChildren(widget))
	{
		size.y += getOuterWidth(*child);

		float width = getPadding(widget).left + getPadding(widget).right + getOuterWidth(*child);
		if (size.x < width)
		{
			size.x = width;
		}
	}

	getSize(widget) = size;

	computeTransform(widget);

	// compute children position
	float currentY = getPadding(widget).top;
	for (Widget* child : getChildren(widget))
	{
		currentY += getMargin(*child).top;

		geometry::Matrix4& childTransform = getTransform(*child);
		childTransform.setIdentity();
		childTransform.translate(geometry::Vector2(getPadding(widget).left + getMargin(*child).left, currentY));
		childTransform = getTransform(widget) * childTransform;

		childrenLayout(*child);

		currentY += getSize(*child).y + getMargin(*child).bottom;
	}
}

void ColumnFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat


