#ifndef FLAT_SHARP_UI_LAYOUT_H
#define FLAT_SHARP_UI_LAYOUT_H

#include "widget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class Layout
{
public:
	static void preLayout(Widget& /*widget*/) {}
	static void layout(Widget& /*widget*/, bool /*computePosition*/) {}
	static void postLayout(Widget& /*widget*/) {}

protected:
	static void childrenPreLayout(Widget& widget);
	static void childrenLayout(Widget& widget);
	static void childrenPostLayout(Widget& widget);
	static void childrenFullLayout(Widget& widget);

	static std::vector<std::shared_ptr<Widget>>& getChildren(Widget& widget);
	static Matrix4& getTransform(Widget& widget);
	static Widget& getParent(Widget& widget);
	static Widget::PositionPolicy& getPositionPolicy(Widget& widget);
	static Widget::Position& getPosition(Widget& widget);
	static Widget::SizePolicy& getSizePolicy(Widget& widget);
	static Widget::Size& getSize(Widget& widget);
	static Widget::Size& getComputedSize(Widget& widget);
	static Widget::Margin& getMargin(Widget& widget);
	static Widget::Padding& getPadding(Widget& widget);
	static Widget::ScrollPosition& getScrollPosition(Widget& widget);
	static Widget::ScrollPosition& getMinScrollPosition(Widget& widget);

	static float getOuterWidth(Widget& widget);
	static float getOuterHeight(Widget& widget);

	static float getParentRelativeRight(Widget& widget);
	static float getParentRelativeBottom(Widget& widget);

	static void computeFixedWidth(Widget& widget);
	static void computeFixedHeight(Widget& widget);
	static void computeExpandWidth(Widget& widget);
	static void computeExpandHeight(Widget& widget);
	static void computeCompressWidth(Widget& widget);
	static void computeCompressHeight(Widget& widget);
	static void computePosition(Widget& widget, Vector2& position);
	static void computePositionX(Widget& widget, Vector2& position);
	static void computePositionY(Widget& widget, Vector2& position);
	static void computeTransform(Widget& widget);
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGET_H



