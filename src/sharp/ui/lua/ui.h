#ifndef FLAT_SHARP_UI_LUA_H
#define FLAT_SHARP_UI_LUA_H

#include <lua5.2/lua.hpp>

namespace flat
{
class Game;
namespace sharp
{
namespace ui
{
class Widget;
class TextWidget;
class WidgetFactory;
namespace lua
{

int open(lua_State* L);
int close(lua_State* L);

int setRootWidget(lua_State* L, Widget* rootWidget);

// Widget methods
int l_Widget_destroy(lua_State* L);
int l_Widget_addChild(lua_State* L);
int l_Widget_removeChild(lua_State* L);
int l_Widget_removeFromParent(lua_State* L);

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

int l_Widget_setVisible(lua_State* L);
int l_Widget_isVisible(lua_State* L);
int l_Widget_hide(lua_State* L);
int l_Widget_show(lua_State* L);

int l_Widget_click(lua_State* L);

// TextWidget only!
int l_TextWidget_setText(lua_State* L);

// static Widget functions
int l_Widget_getRoot(lua_State* L);

int l_Widget_makeImage(lua_State* L);
int l_Widget_makeFixedSize(lua_State* L);
int l_Widget_makeLineFlow(lua_State* L);
int l_Widget_makeColumnFlow(lua_State* L);
int l_Widget_makeText(lua_State* L);

// private
Widget* getRootWidget(lua_State* L);
Widget* getWidget(lua_State* L, int index);
TextWidget* getTextWidget(lua_State* L, int index);
void pushWidget(lua_State* L, Widget* widget);
WidgetFactory* getWidgetFactory(lua_State* L);

} // lua
} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_LUA_H



