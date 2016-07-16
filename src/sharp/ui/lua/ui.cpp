#include "ui.h"
#include "../../../lua/lua.h"
#include "../../../lua/reference.h"
#include "../../../lua/table.h"
#include "../widget.h"
#include "../widgetfactory.h"

namespace flat
{
namespace sharp
{
namespace ui
{
namespace lua
{

static char widgetFactoryRegistryIndex = 'W';
static char rootWidgetRegistryIndex = 'R';

int open(lua_State* L)
{
	// Widget metatable
	luaL_newmetatable(L, "Flat.Widget");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Widget_lib_m[] = {
		{"addChild",          l_Widget_addChild},
		
		{"setSizePolicy",     l_Widget_setSizePolicy},
		{"getSizePolicy",     l_Widget_getSizePolicy},
		{"setSize",           l_Widget_setSize},
		{"getSize",           l_Widget_getSize},
		
		{"setPositionPolicy", l_Widget_setPositionPolicy},
		{"getPositionPolicy", l_Widget_getPositionPolicy},
		
		{"click",             l_Widget_click},
		
		{nullptr, nullptr}
	};
	
	luaL_setfuncs(L, Widget_lib_m, 0);
	
	lua_pop(L, 1);
	
	// Widget static methods
	static const luaL_Reg Widget_lib_s[] = {
		{"getRoot",       l_Widget_getRoot},
		
		{"makeImage",     l_Widget_makeImage},
		{"makeFixedSize", l_Widget_makeFixedSize},
		{"makeLineFlow",  l_Widget_makeLineFlow},
		
		{nullptr, nullptr}
	};
	
	luaL_newlib(L, Widget_lib_s);
	
	lua_pushstring(L, "PositionPolicy");
	const flat::lua::table::KeyValuePair<int> positionPolicyTable[] = {
		{"LEFT",         Widget::PositionPolicy::LEFT},
		{"CENTER_X",     Widget::PositionPolicy::CENTER_X},
		{"RIGHT",        Widget::PositionPolicy::RIGHT},
		{"FLOW_X",       Widget::PositionPolicy::FLOW_X},
		
		{"TOP",          Widget::PositionPolicy::TOP},
		{"CENTER_Y",     Widget::PositionPolicy::CENTER_Y},
		{"BOTTOM",       Widget::PositionPolicy::BOTTOM},
		{"FLOW_Y",       Widget::PositionPolicy::FLOW_Y},
		
		{"TOP_LEFT",     Widget::PositionPolicy::TOP_LEFT},
		{"TOP_RIGHT",    Widget::PositionPolicy::TOP_RIGHT},
		{"BOTTOM_LEFT",  Widget::PositionPolicy::BOTTOM_LEFT},
		{"BOTTOM_RIGHT", Widget::PositionPolicy::BOTTOM_RIGHT},
		{"CENTER",       Widget::PositionPolicy::CENTER},
		{"COLUMN_FLOW",  Widget::PositionPolicy::COLUMN_FLOW},
		{"LINE_FLOW",    Widget::PositionPolicy::LINE_FLOW},
		
		{nullptr, 0},
	};
	flat::lua::table::pushTable(L, positionPolicyTable);
	lua_settable(L, -3);
	
	lua_setglobal(L, "Widget");
	
	// TODO actual full userdata with __gc for WidgetFactory
	Game* game = flat::lua::getGame(L);
	WidgetFactory* widgetFactory = new WidgetFactory(*game);
	lua_pushlightuserdata(L, &widgetFactoryRegistryIndex);
	lua_pushlightuserdata(L, widgetFactory);
	lua_settable(L, LUA_REGISTRYINDEX);
	
	return 0;
}

int close(lua_State* L)
{
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	delete widgetFactory;
	lua_pushlightuserdata(L, &widgetFactoryRegistryIndex);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	return 0;
}

int setRootWidget(lua_State* L, Widget* rootWidget)
{
	lua_pushlightuserdata(L, &rootWidgetRegistryIndex);
	lua_pushlightuserdata(L, rootWidget);
	lua_settable(L, LUA_REGISTRYINDEX);
	return 0;
}

// Widget methods

int l_Widget_addChild(lua_State* L)
{
	Widget* parent = getWidget(L, 1);
	Widget* child = getWidget(L, 2);
	parent->addChild(child);
	return 0;
}

int l_Widget_setSizePolicy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::SizePolicy sizePolicy = static_cast<Widget::SizePolicy>(luaL_checkint(L, 2));
	widget->setSizePolicy(sizePolicy);
	return 0;
}

int l_Widget_getSizePolicy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::SizePolicy sizePolicy = widget->getSizePolicy();
	lua_pushinteger(L, sizePolicy);
	return 1;
}

int l_Widget_setSize(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	int width = luaL_checkint(L, 2);
	int height = luaL_checkint(L, 3);
	Widget::Size size(width, height);
	widget->setSize(size);
	return 0;
}

int l_Widget_getSize(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	const Widget::Size& size = widget->getSize();
	lua_pushinteger(L, size.x);
	lua_pushinteger(L, size.y);
	return 2;
}

int l_Widget_setPositionPolicy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::PositionPolicy positionPolicy = static_cast<Widget::PositionPolicy>(luaL_checkint(L, 2));
	widget->setPositionPolicy(positionPolicy);
	return 0;
}

int l_Widget_getPositionPolicy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::PositionPolicy positionPolicy = widget->getPositionPolicy();
	lua_pushinteger(L, positionPolicy);
	return 1;
}

int l_Widget_click(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	flat::lua::SharedReference<LUA_TFUNCTION> ref(L, 2);
	widget->click.on(
		[ref](Widget* w, bool& eventHandled)
		{
			lua_State* L = ref.push(); // TODO WRONG LUA STATE
			luaL_checktype(L, -1, LUA_TFUNCTION);
			pushWidget(L, w);
			lua_call(L, 1, 1);
			eventHandled |= lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
	);
	return 0;
}

// static Widget functions

int l_Widget_getRoot(lua_State* L)
{
	Widget* rootWidget = getRootWidget(L);
	pushWidget(L, rootWidget);
	return 1;
}

int l_Widget_makeImage(lua_State* L)
{
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	std::string fileName = luaL_checkstring(L, 1);
	Widget* widget = widgetFactory->makeImage(fileName);
	pushWidget(L, widget);
	return 1;
}

int l_Widget_makeFixedSize(lua_State* L)
{
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	int x = luaL_checkint(L, 1);
	int y = luaL_checkint(L, 2);
	Widget::Size size(x, y);
	Widget* widget = widgetFactory->makeFixedSize(size);
	pushWidget(L, widget);
	return 1;
}

int l_Widget_makeLineFlow(lua_State* L)
{
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	Widget* widget = widgetFactory->makeLineFlow();
	pushWidget(L, widget);
	return 1;
}

// private

Widget* getRootWidget(lua_State* L)
{
	lua_pushlightuserdata(L, &rootWidgetRegistryIndex);
	lua_gettable(L, LUA_REGISTRYINDEX);
	Widget* rootWidget = static_cast<Widget*>(lua_touserdata(L, -1));
	FLAT_ASSERT(rootWidget);
	lua_pop(L, 1);
	return rootWidget;
}

Widget* getWidget(lua_State* L, int index)
{
	return *static_cast<Widget**>(luaL_checkudata(L, index, "Flat.Widget"));
}

void pushWidget(lua_State* L, Widget* widget)
{
	if (widget != nullptr)
	{
		Widget** widgetPointer = static_cast<Widget**>(lua_newuserdata(L, sizeof(Widget*)));
		*widgetPointer = widget;
		luaL_getmetatable(L, "Flat.Widget");
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}
}

WidgetFactory* getWidgetFactory(lua_State* L)
{
	lua_pushlightuserdata(L, &widgetFactoryRegistryIndex);
	lua_gettable(L, LUA_REGISTRYINDEX);
	WidgetFactory* widgetFactory = static_cast<WidgetFactory*>(lua_touserdata(L, -1));
	FLAT_ASSERT(widgetFactory);
	lua_pop(L, 1);
	return widgetFactory;
}

} // lua
} // ui
} // sharp
} // flat



