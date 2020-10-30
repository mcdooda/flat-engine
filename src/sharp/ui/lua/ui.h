#ifndef FLAT_SHARP_UI_LUA_H
#define FLAT_SHARP_UI_LUA_H

#include <memory>

#include "misc/slot.h"
#include "misc/lua/vector2.h"
#include "lua/push.h"

#include "input/gamepadbuttons.h"

struct lua_State;

namespace flat
{
class Flat;
namespace lua
{
class Lua;
}
namespace sharp
{
namespace ui
{
class CanvasWidget;
class FocusableWidget;
class TextInputWidget;
class NumberInputWidget;
class TextWidget;
class RootWidget;
class Widget;
class WidgetFactory;
namespace lua
{

int open(Flat& flat, flat::lua::Lua& lua);
int close(lua_State* L);

// Widget methods
int l_Widget_addChild(lua_State* L);
int l_Widget_removeChild(lua_State* L);
int l_Widget_removeChildAtIndex(lua_State* L);
int l_Widget_removeAllChildren(lua_State* L);
int l_Widget_getChildrenCount(lua_State* L);
int l_Widget_getChildAtIndex(lua_State* L);
int l_Widget_removeFromParent(lua_State* L);
int l_Widget_getParent(lua_State* L);

int l_Widget_setSizePolicy(lua_State* L);
int l_Widget_setSizePolicyX(lua_State* L);
int l_Widget_setSizePolicyY(lua_State* L);
int l_Widget_getSizePolicy(lua_State* L);
int l_Widget_setSize(lua_State* L);
int l_Widget_getSize(lua_State* L);
int l_Widget_getComputedSize(lua_State* L);

int l_Widget_setPositionPolicy(lua_State* L);
int l_Widget_getPositionPolicy(lua_State* L);
int l_Widget_setPosition(lua_State* L);
int l_Widget_getPosition(lua_State* L);
int l_Widget_getRelativePosition(lua_State* L);

int l_Widget_setRotation(lua_State* L);
int l_Widget_setRotationZ(lua_State* L);
int l_Widget_getRotation(lua_State* L);

int l_Widget_setMargin(lua_State* L);
int l_Widget_getMargin(lua_State* L);

int l_Widget_setPadding(lua_State* L);
int l_Widget_getPadding(lua_State* L);

int l_Widget_setBackground(lua_State* L);
int l_Widget_setBackgroundRepeat(lua_State* L);
int l_Widget_setBackgroundColor(lua_State* L);
int l_Widget_setBackgroundPosition(lua_State* L);
int l_Widget_setBackgroundSize(lua_State* L);

int l_Widget_setVisible(lua_State* L);
int l_Widget_getVisible(lua_State* L);
int l_Widget_hide(lua_State* L);
int l_Widget_show(lua_State* L);

int l_Widget_setFocusable(lua_State* L);

int l_Widget_setAllowScrollX(lua_State* L);
int l_Widget_getAllowScrollX(lua_State* L);
int l_Widget_setAllowScrollY(lua_State* L);
int l_Widget_getAllowScrollY(lua_State* L);
int l_Widget_setAllowScroll(lua_State* L);
int l_Widget_getAllowScroll(lua_State* L);

int l_Widget_setAllowDragScrolling(lua_State* L);
int l_Widget_getAllowDragScrolling(lua_State* L);

int l_Widget_setRestrictScrollX(lua_State* L);
int l_Widget_getRestrictScrollX(lua_State* L);
int l_Widget_setRestrictScrollY(lua_State* L);
int l_Widget_getRestrictScrollY(lua_State* L);
int l_Widget_setRestrictScroll(lua_State* L);
int l_Widget_getRestrictScroll(lua_State* L);

int l_Widget_setScrollPosition(lua_State* L);
int l_Widget_getScrollPosition(lua_State* L);

int l_Widget_scrollInDirection(lua_State* L);

int l_Widget_copy(lua_State* L);
int l_Widget_paste(lua_State* L);
int l_Widget_undo(lua_State* L);
int l_Widget_redo(lua_State* L);
int l_Widget_click(lua_State* L);
int l_Widget_rightClick(lua_State* L);
int l_Widget_mouseDown(lua_State* L);
int l_Widget_mouseUp(lua_State* L);
int l_Widget_mouseMove(lua_State* L);
int l_Widget_mouseWheel(lua_State* L);
int l_Widget_mouseEnter(lua_State* L);
int l_Widget_mouseLeave(lua_State* L);
int l_Widget_scroll(lua_State* L);
int l_Widget_beforeDrag(lua_State* L);
int l_Widget_dragged(lua_State* L);

int l_Widget_drag(lua_State* L);
int l_Widget_drop(lua_State* L);

int l_Widget_gamepadButtonPressed(lua_State* L);
int l_Widget_gamepadButtonReleased(lua_State* L);

// TextWidget only
int l_TextWidget_setText(lua_State* L);
int l_TextWidget_getText(lua_State* L);
int l_TextWidget_setTextColor(lua_State* L);

// TextInputWidget only
int l_TextInputWidget_valueChanged(lua_State* L);
int l_TextInputWidget_submit(lua_State* L);

// NumberInputWidget only
int l_NumberInputWidget_setValue(lua_State* L);
int l_NumberInputWidget_getValue(lua_State* L);
int l_NumberInputWidget_setPrecision(lua_State* L);
int l_NumberInputWidget_setMin(lua_State* L);
int l_NumberInputWidget_setMax(lua_State* L);
int l_NumberInputWidget_setRange(lua_State* L);

// FocusableWidget only
int l_FocusableWidget_focus(lua_State* L);
int l_FocusableWidget_blur(lua_State* L);

// CanvasWidget only
int l_CanvasWidget_draw(lua_State* L);
int l_CanvasWidget_redraw(lua_State* L);
int l_CanvasWidget_clear(lua_State* L);
int l_CanvasWidget_drawLine(lua_State* L);
int l_CanvasWidget_drawBezier(lua_State* L);
int l_CanvasWidget_getBounds(lua_State* L);

// static Widget functions
int l_Widget_getRoot(lua_State* L);
int l_Widget_focus(lua_State* L);

int l_Widget_makeImage(lua_State* L);
int l_Widget_makeFixedSize(lua_State* L);
int l_Widget_makeExpand(lua_State* L);
int l_Widget_makeCompress(lua_State* L);
int l_Widget_makeLineFlow(lua_State* L);
int l_Widget_makeColumnFlow(lua_State* L);
int l_Widget_makeText(lua_State* L);
int l_Widget_makeTextInput(lua_State* L);
int l_Widget_makeNumberInput(lua_State* L);
int l_Widget_makeCanvas(lua_State* L);

// private
Widget& getWidget(lua_State* L, int index);
Widget& getFocusableWidget(lua_State* L, int index);

template <class T>
T& getWidgetOfType(lua_State* L, int index);

template <class T, class... Args>
int addWidgetCallback(lua_State* L, Slot<Widget*, Args...> T::* slot);

template <class T, class... Args>
int addCopyWidgetCallback(lua_State* L, Slot <Widget*, Args...> T::* slot);

template <class T>
int addPropagatedMouseWidgetCallback(lua_State* L, Slot<Widget*, bool&> T::* slot);

int addPropagatedMouseWheelWidgetCallback(lua_State* L, Slot<Widget*, bool&, const Vector2&> Widget::* slot);

int addGamepadButtonEventCallback(lua_State* L, Slot<Widget*, input::GamepadIndex, input::GamepadButton> Widget::* slot);

int addBeforeDragWidgetCallback(lua_State* L, Slot<Widget*, Vector2&> Widget::* slot);

void pushWidget(lua_State* L, const std::shared_ptr<Widget>& widget);
WidgetFactory& getWidgetFactory(lua_State* L);
RootWidget& getRootWidget(lua_State* L);

} // lua
} // ui
} // sharp
} // flat

FLAT_LUA_PUSHABLE_TYPE(const std::shared_ptr<flat::sharp::ui::Widget>&, flat::sharp::ui::lua::pushWidget(L, value))

#endif // FLAT_SHARP_UI_LUA_H



