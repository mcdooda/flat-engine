#include "sharp/message/message.h"

#include "flat.h"

namespace flat
{
namespace sharp
{
namespace message
{

Message::Message(Flat& flat) :
	m_flat(flat)
{
	reset();
	resetCallbacks();
}

void Message::reset()
{
	lua_State* L = m_flat.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_flat.lua->pushVariable({"flat", "ui", "confirm"});
		m_confirm.set(L, -1);

		m_flat.lua->pushVariable({"flat", "ui", "choice"});
		m_choice.set(L, -1);

		m_flat.lua->pushVariable({"flat", "ui", "prompt"});
		m_prompt.set(L, -1);

		lua_pop(L, 3);
	}
}

void Message::update()
{
	if (m_messageThread.isRunning())
	{
		lua_State* L = m_flat.lua->state;
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			int status = m_messageThread.update(2);
			switch (status)
			{
			case LUA_YIELD:
				// nothing to to
				break;

			case LUA_OK:
				if (lua_isnil(L, -2))
				{
					handleNil();
				}
				else
				{
					handleResult(L);
				}
				break;

			default:
				FLAT_ASSERT(false);
			}

			lua_pop(L, 2);
		}
	}
}

bool Message::confirm(const std::string& message, std::function<void(bool)> callback)
{
	if (isBusy())
		return false;

	lua_State* L = m_flat.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_confirm.push(L);
		m_messageThread.set(L, -1);

		lua_pushstring(L, message.c_str());

		m_messageThread.start(1);

		lua_pop(L, 1);
	}

	m_currentMessageType = MessageType::CONFIRM;

	FLAT_ASSERT(m_confirmCallback == nullptr);
	m_confirmCallback = callback;

	return true;
}

bool Message::choice(const std::string& message, const std::vector<std::string>& choices, int defaultValue, std::function<void(int, const std::string&)> callback)
{
	if (isBusy())
		return false;

	lua_State* L = m_flat.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_choice.push(L);
		m_messageThread.set(L, -1);

		lua_pushstring(L, message.c_str());

		lua_createtable(L, static_cast<int>(choices.size()), 0);
		for (int i = 0; i < choices.size(); ++i)
		{
			lua_pushstring(L, choices[i].c_str());
			lua_rawseti(L, -2, i + 1);
		}

		if (defaultValue != ChoiceResult::NONE)
		{
			lua_pushstring(L, choices[defaultValue].c_str());
		}
		else
		{
			lua_pushnil(L);
		}

		m_messageThread.start(2);

		lua_pop(L, 1);
	}

	m_currentMessageType = MessageType::CHOICE;

	FLAT_ASSERT(m_choiceCallback == nullptr);
	m_choiceCallback = callback;

	return true;
}

bool Message::prompt(const std::string& message, const std::string& defaultValue, std::function<void(bool, const std::string&)> callback)
{
	if (isBusy())
		return false;

	lua_State* L = m_flat.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_choice.push(L);
		m_messageThread.set(L, -1);

		lua_pushstring(L, message.c_str());

		lua_pushstring(L, defaultValue.c_str());

		m_messageThread.start(2);

		lua_pop(L, 1);
	}

	m_currentMessageType = MessageType::PROMPT;

	FLAT_ASSERT(m_promptCallback == nullptr);
	m_promptCallback = callback;

	return true;
}

void Message::resetCallbacks()
{
	m_currentMessageType = MessageType::NONE;
	m_confirmCallback = nullptr;
	m_choiceCallback = nullptr;
	m_promptCallback = nullptr;
}

void Message::handleNil()
{
	FLAT_ASSERT(m_currentMessageType != MessageType::NONE);
	switch (m_currentMessageType)
	{
	case MessageType::CONFIRM:
		FLAT_ASSERT(m_confirmCallback != nullptr);
		m_confirmCallback(false);
		break;

	case MessageType::CHOICE:
		FLAT_ASSERT(m_choiceCallback != nullptr);
		m_choiceCallback(ChoiceResult::NONE, "");
		break;

	case MessageType::PROMPT:
		FLAT_ASSERT(m_promptCallback != nullptr);
		m_promptCallback(false, "");
		break;

	default:
		FLAT_ASSERT(false);
	}
	resetCallbacks();
}

void Message::handleResult(lua_State* L)
{
	FLAT_ASSERT(m_currentMessageType != MessageType::NONE);
	switch (m_currentMessageType)
	{
	case MessageType::CONFIRM:
		{
			FLAT_ASSERT(m_confirmCallback != nullptr);
			const bool result = lua_toboolean(L, -2);
			m_confirmCallback(result);
		}
		break;

	case MessageType::CHOICE:
		{
			FLAT_ASSERT(m_choiceCallback != nullptr);
			const char* result = luaL_checkstring(L, -2);
			const int resultIndex = static_cast<int>(luaL_checkinteger(L, -1));
			m_choiceCallback(resultIndex, result);
		}
		break;

	case MessageType::PROMPT:
		{
			FLAT_ASSERT(m_promptCallback != nullptr);
			const char* result = luaL_checkstring(L, -2);
			m_promptCallback(true, result);
		}
		break;

	default:
		FLAT_ASSERT(false);
	}
	resetCallbacks();
}

} // notify
} // sharp
} // flat


