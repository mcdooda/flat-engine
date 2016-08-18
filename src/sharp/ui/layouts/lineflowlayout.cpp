#include "lineflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void LineFlowLayout::preLayout(Widget& widget)
{
	childrenPreLayout(widget);
}

void LineFlowLayout::layout(Widget& widget)
{
	// compute size
	geometry::Vector2 size(getPadding(widget).left + getPadding(widget).right, getPadding(widget).top + getPadding(widget).bottom);
	for (Widget* child : getChildren(widget))
	{
		size.x += getOuterWidth(*child);

		float height = getPadding(widget).top + getPadding(widget).bottom + getOuterHeight(*child);
		if (size.y < height)
		{
			size.y = height;
		}
	}

	getSize(widget) = size;

	computeTransform(widget);

	// compute children position
	float currentX = getPadding(widget).left;
	for (Widget* child : getChildren(widget))
	{
		currentX += getMargin(*child).left;

		geometry::Matrix4& childTransform = getTransform(*child);
		childTransform.setIdentity();
		childTransform.translate(geometry::Vector2(currentX, getPadding(widget).top + getMargin(*child).top));
		childTransform = getTransform(widget) * childTransform;

		childrenLayout(*child);

		currentX += getSize(*child).x + getMargin(*child).right;
	}
}

void LineFlowLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat

