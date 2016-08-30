#include "ui.h"
#include "../../../flat/game.h"
#include "../../../lua/lua.h"
#include "../../../lua/reference.h"
#include "../../../lua/table.h"
#include "../widget.h"
#include "../textwidget.h"
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
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	// Widget metatable
	luaL_newmetatable(L, "Flat.Widget");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Widget_lib_m[] = {
		{"destroy",             l_Widget_destroy},
		
		{"addChild",            l_Widget_addChild},
		{"removeChild",         l_Widget_removeChild},
		{"removeFromParent",    l_Widget_removeFromParent},
		
		{"setSizePolicy",       l_Widget_setSizePolicy},
		{"setSizePolicyX",      l_Widget_setSizePolicyX},
		{"setSizePolicyY",      l_Widget_setSizePolicyY},
		{"getSizePolicy",       l_Widget_getSizePolicy},
		{"setSize",             l_Widget_setSize},
		{"getSize",             l_Widget_getSize},
		
		{"setPositionPolicy",   l_Widget_setPositionPolicy},
		{"getPositionPolicy",   l_Widget_getPositionPolicy},
		{"setPosition",         l_Widget_setPosition},
		{"getPosition",         l_Widget_getPosition},
		
		{"setRotation",         l_Widget_setRotation},
		{"setRotationZ",        l_Widget_setRotationZ},
		{"getRotation",         l_Widget_getRotation},
		
		{"setBackground",       l_Widget_setBackground},
		{"setBackgroundRepeat", l_Widget_setBackgroundRepeat},
		{"setBackgroundColor",  l_Widget_setBackgroundColor},
		
		{"setVisible",          l_Widget_setVisible},
		{"getVisible",          l_Widget_isVisible},
		{"hide",                l_Widget_hide},
		{"show",                l_Widget_show},
		
		{"click",               l_Widget_click},
		{"mouseMove",           l_Widget_mouseMove},
		{"mouseEnter",          l_Widget_mouseEnter},
		{"mouseLeave",          l_Widget_mouseLeave},
		
		{"setText",             l_TextWidget_setText},
		
		{nullptr, nullptr}
	};
	
	luaL_setfuncs(L, Widget_lib_m, 0);
	
	lua_pop(L, 1);
	
	// Widget static methods
	static const luaL_Reg Widget_lib_s[] = {
		{"getRoot",        l_Widget_getRoot},
		
		{"makeImage",      l_Widget_makeImage},
		{"makeFixedSize",  l_Widget_makeFixedSize},
		{"makeLineFlow",   l_Widget_makeLineFlow},
		{"makeColumnFlow", l_Widget_makeColumnFlow},
		{"makeText",       l_Widget_makeText},
		
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

	lua_pushstring(L, "BackgroundRepeat");
	const flat::lua::table::KeyValuePair<int> backgroundRepeatTable[] = {
		{ "SCALED", Widget::BackgroundRepeat::SCALED },
		{ "REPEAT", Widget::BackgroundRepeat::REPEAT },

		{ nullptr, 0 },
	};
	flat::lua::table::pushTable(L, backgroundRepeatTable);
	lua_settable(L, -3);
	
	lua_setglobal(L, "Widget");
	
	// TODO actual full userdata with __gc for WidgetFactory
	Game* game = flat::lua::getGame(L);
	WidgetFactory* widgetFactory = new WidgetFactory(*game);
	lua_pushlightuserdata(L, widgetFactory);
	lua_rawsetp(L, LUA_REGISTRYINDEX, &widgetFactoryRegistryIndex);
	
	return 0;
}

int close(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	delete widgetFactory;
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, &widgetFactoryRegistryIndex);
	return 0;
}

int setRootWidget(lua_State* L, Widget* rootWidget)
{
	lua_pushlightuserdata(L, rootWidget);
	lua_rawsetp(L, LUA_REGISTRYINDEX, &rootWidgetRegistryIndex);
	return 0;
}

// Widget methods

int l_Widget_destroy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	delete widget;
	return 0;
}

int l_Widget_addChild(lua_State* L)
{
	Widget* parent = getWidget(L, 1);
	Widget* child = getWidget(L, 2);
	parent->addChild(child);
	return 0;
}

int l_Widget_removeChild(lua_State* L)
{
	Widget* parent = getWidget(L, 1);
	Widget* child = getWidget(L, 2);
	parent->removeChild(child);
	return 0;
}

int l_Widget_removeFromParent(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	widget->removeFromParent();
	return 0;
}

int l_Widget_setSizePolicy(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::SizePolicy sizePolicy = static_cast<Widget::SizePolicy>(luaL_checkint(L, 2));
	widget->setSizePolicy(sizePolicy);
	return 0;
}

int l_Widget_setSizePolicyX(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::SizePolicy sizePolicyX = static_cast<Widget::SizePolicy>(luaL_checkint(L, 2));
	widget->setSizePolicyX(sizePolicyX);
	return 0;
}

int l_Widget_setSizePolicyY(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::SizePolicy sizePolicyY = static_cast<Widget::SizePolicy>(luaL_checkint(L, 2));
	widget->setSizePolicyY(sizePolicyY);
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
	float width = static_cast<float>(luaL_checknumber(L, 2));
	float height = static_cast<float>(luaL_checknumber(L, 3));
	Widget::Size size(width, height);
	widget->setSize(size);
	return 0;
}

int l_Widget_getSize(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	const Widget::Size& size = widget->getSize();
	lua_pushnumber(L, size.x);
	lua_pushnumber(L, size.y);
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

int l_Widget_setPosition(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	Widget::Position position(x, y);
	widget->setPosition(position);
	return 0;
}

int l_Widget_getPosition(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	const Widget::Position& position = widget->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	return 2;
}

int l_Widget_setRotation(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	float rotationX = static_cast<float>(luaL_checknumber(L, 2));
	float rotationY = static_cast<float>(luaL_checknumber(L, 3));
	float rotationZ = static_cast<float>(luaL_checknumber(L, 4));
	widget->setRotation(Widget::Rotation(rotationX, rotationY, rotationZ));
	return 0;
}

int l_Widget_setRotationZ(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	float rotationZ = static_cast<float>(luaL_checknumber(L, 2));
	widget->setRotationZ(rotationZ);
	return 0;
}

int l_Widget_getRotation(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	const Widget::Rotation& rotation = widget->getRotation();
	lua_pushnumber(L, rotation.x);
	lua_pushnumber(L, rotation.y);
	lua_pushnumber(L, rotation.z);
	return 3;
}

int l_Widget_setBackground(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	const char* backgroundFileName = luaL_checkstring(L, 2);
	Game* game = flat::lua::getGame(L);
	std::shared_ptr<const flat::video::FileTexture> background = game->video->getTexture(backgroundFileName);
	widget->setBackground(background);
	return 0;
}

int l_Widget_setBackgroundRepeat(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	Widget::BackgroundRepeat backgroundRepeat = static_cast<Widget::BackgroundRepeat>(luaL_checkint(L, 2));
	widget->setBackgroundRepeat(backgroundRepeat);
	return 0;
}

int l_Widget_setBackgroundColor(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	uint32_t color = luaL_checkint(L, 2);
	float r = static_cast<float>((color >> 24) & 0xFF) / 255.f;
	float g = static_cast<float>((color >> 16) & 0xFF) / 255.f;
	float b = static_cast<float>((color >> 8 ) & 0xFF) / 255.f;
	float a = static_cast<float>((color >> 0 ) & 0xFF) / 255.f;
	flat::video::Color backgroundColor(r, g, b, a);
	widget->setBackgroundColor(backgroundColor);
	return 0;
}

int l_Widget_setVisible(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	bool visible = lua_toboolean(L, 2) == 1;
	widget->setVisible(visible);
	return 0;
}

int l_Widget_isVisible(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	bool visible = widget->isVisible();
	lua_pushboolean(L, visible);
	return 1;
}

int l_Widget_hide(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	widget->hide();
	return 0;
}

int l_Widget_show(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	widget->show();
	return 0;
}

int l_Widget_click(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	FLAT_ASSERT(L == flat::lua::getMainThread(L));
	flat::lua::SharedReference<LUA_TFUNCTION> ref(L, 2);
	input::Mouse* mouse = flat::lua::getInput(L)->mouse;
	widget->click.on(
		[L, ref, mouse](Widget* w, bool& eventHandled)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			Vector2 relativePosition = w->getRelativePosition(mouse->getPosition());
			ref.push(L);
			luaL_checktype(L, -1, LUA_TFUNCTION);
			pushWidget(L, w);
			lua_pushnumber(L, relativePosition.x);
			lua_pushnumber(L, relativePosition.y);
			lua_call(L, 3, 1);
			eventHandled = eventHandled || lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
	);
	return 0;
}

int l_Widget_mouseMove(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	FLAT_ASSERT(L == flat::lua::getMainThread(L));
	flat::lua::SharedReference<LUA_TFUNCTION> ref(L, 2);
	input::Mouse* mouse = flat::lua::getInput(L)->mouse;
	widget->mouseMove.on(
		[L, ref, mouse](Widget* w, bool& eventHandled)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			Vector2 relativePosition = w->getRelativePosition(mouse->getPosition());
			ref.push(L);
			luaL_checktype(L, -1, LUA_TFUNCTION);
			pushWidget(L, w);
			lua_pushnumber(L, relativePosition.x);
			lua_pushnumber(L, relativePosition.y);
			lua_call(L, 3, 1);
			eventHandled = eventHandled || lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
	);
	return 0;
}

int l_Widget_mouseEnter(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	FLAT_ASSERT(L == flat::lua::getMainThread(L));
	flat::lua::SharedReference<LUA_TFUNCTION> ref(L, 2);
	widget->mouseEnter.on(
		[L, ref](Widget* w)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			ref.push(L);
			luaL_checktype(L, -1, LUA_TFUNCTION);
			pushWidget(L, w);
			lua_call(L, 1, 0);
		}
	);
	return 0;
}

int l_Widget_mouseLeave(lua_State* L)
{
	Widget* widget = getWidget(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	FLAT_ASSERT(L == flat::lua::getMainThread(L));
	flat::lua::SharedReference<LUA_TFUNCTION> ref(L, 2);
	widget->mouseLeave.on(
		[L, ref](Widget* w)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			ref.push(L);
			luaL_checktype(L, -1, LUA_TFUNCTION);
			pushWidget(L, w);
			lua_call(L, 1, 0);
		}
	);
	return 0;
}

int l_TextWidget_setText(lua_State* L)
{
	TextWidget* textWidget = getTextWidget(L, 1);
	const char* text = luaL_checkstring(L, 2);
	textWidget->setText(text);
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
	float width = static_cast<float>(luaL_checknumber(L, 1));
	float height = static_cast<float>(luaL_checknumber(L, 2));
	Widget::Size size(width, height);
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

int l_Widget_makeColumnFlow(lua_State* L)
{
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	Widget* widget = widgetFactory->makeColumnFlow();
	pushWidget(L, widget);
	return 1;
}

int l_Widget_makeText(lua_State* L)
{
	const char* text = luaL_checkstring(L, 1);
	const char* fileName = luaL_checkstring(L, 2);
	int fontSize = luaL_checkint(L, 3);
	WidgetFactory* widgetFactory = getWidgetFactory(L);
	Widget* widget = widgetFactory->makeText(text, fileName, fontSize);
	pushWidget(L, widget);
	return 1;
}

// private

Widget* getRootWidget(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_rawgetp(L, LUA_REGISTRYINDEX, &rootWidgetRegistryIndex);
	Widget* rootWidget = static_cast<Widget*>(lua_touserdata(L, -1));
	FLAT_ASSERT(rootWidget != nullptr);
	lua_pop(L, 1);
	return rootWidget;
}

Widget* getWidget(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return *static_cast<Widget**>(luaL_checkudata(L, index, "Flat.Widget"));
}

TextWidget* getTextWidget(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	Widget* widget = getWidget(L, index);
	TextWidget* textWidget = dynamic_cast<TextWidget*>(widget);
	if (!textWidget)
	{
		luaL_error(L, "TextWidget required, Widget given");
	}
	return textWidget;
}

void pushWidget(lua_State* L, Widget* widget)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
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
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_rawgetp(L, LUA_REGISTRYINDEX, &widgetFactoryRegistryIndex);
	WidgetFactory* widgetFactory = static_cast<WidgetFactory*>(lua_touserdata(L, -1));
	FLAT_ASSERT(widgetFactory != nullptr);
	lua_pop(L, 1);
	return widgetFactory;
}

} // lua
} // ui
} // sharp
} // flat



