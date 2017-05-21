#ifndef FLAT_SHARP_UI_LUA_H
#define FLAT_SHARP_UI_LUA_H

#include <memory>
#include "../widget.h"
#include "../../../lua/sharedcppreference.h"

namespace flat
{
class Game;
namespace sharp
{
namespace ui
{
class RootWidget;
class TextWidget;
class FocusableWidget;
class WidgetFactory;
namespace lua
{

int open(lua_State* L);
int close(lua_State* L);

// Widget methods
int l_Widget_addChild(lua_State* L);
int l_Widget_removeChild(lua_State* L);
int l_Widget_removeFromParent(lua_State* L);
int l_Widget_removeAllChildren(lua_State* L);

int l_Widget_setSizePolicy(lua_State* L);
int l_Widget_setSizePolicyX(lua_State* L);
int l_Widget_setSizePolicyY(lua_State* L);
int l_Widget_getSizePolicy(lua_State* L);
int l_Widget_setSize(lua_State* L);
int l_Widget_getSize(lua_State* L);

int l_Widget_setPositionPolicy(lua_State* L);
int l_Widget_getPositionPolicy(lua_State* L);
int l_Widget_setPosition(lua_State* L);
int l_Widget_getPosition(lua_State* L);

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

int l_Widget_setVisible(lua_State* L);
int l_Widget_getVisible(lua_State* L);
int l_Widget_hide(lua_State* L);
int l_Widget_show(lua_State* L);

int l_Widget_click(lua_State* L);
int l_Widget_mouseMove(lua_State* L);
int l_Widget_mouseEnter(lua_State* L);
int l_Widget_mouseLeave(lua_State* L);

// TextWidget only!
int l_TextWidget_setText(lua_State* L);
int l_TextWidget_setTextColor(lua_State* L);

// FocusableWidget only!
int l_FocusableWidget_focus(lua_State* L);
int l_FocusableWidget_blur(lua_State* L);

// static Widget functions
int l_Widget_getRoot(lua_State* L);

int l_Widget_makeImage(lua_State* L);
int l_Widget_makeFixedSize(lua_State* L);
int l_Widget_makeLineFlow(lua_State* L);
int l_Widget_makeColumnFlow(lua_State* L);
int l_Widget_makeText(lua_State* L);
int l_Widget_makeTextInput(lua_State* L);

// private
Widget& getWidget(lua_State* L, int index);
TextWidget& getTextWidget(lua_State* L, int index);
FocusableWidget& getFocusableWidget(lua_State* L, int index);
void pushWidget(lua_State* L, const std::shared_ptr<Widget>& widget);
WidgetFactory& getWidgetFactory(lua_State* L);

} // lua
} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_LUA_H



