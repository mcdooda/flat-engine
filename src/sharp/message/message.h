#ifndef FLAT_SHARP_MESSAGE_MESSAGE_H
#define FLAT_SHARP_MESSAGE_MESSAGE_H

#include <functional>

#include "lua/uniqueluareference.h"
#include "lua/thread.h"

namespace flat
{
class Flat;
namespace sharp
{
namespace message
{

class Message
{
	public:
		enum ChoiceResult
		{
			NONE = -1
		};

	private:
		enum class MessageType
		{
			NONE,
			CONFIRM,
			CHOICE,
			PROMPT
		};

	public:
		Message(Flat& flat);

		void reset();

		void update();

		inline bool isBusy() const { return m_currentMessageType != MessageType::NONE; }

		bool confirm(const std::string& message, std::function<void(bool)> callback);
		bool choice(const std::string& message, const std::vector<std::string>& choices, int defaultValue, std::function<void(int, const std::string&)> callback);
		bool prompt(const std::string& message, const std::string& defaultValue, std::function<void(bool, const std::string&)> callback);

	private:
		void resetCallbacks();

		void handleNil();
		void handleResult(lua_State* L);

	private:
		Flat& m_flat;

		MessageType m_currentMessageType;

		lua::UniqueLuaReference<LUA_TFUNCTION> m_confirm;
		std::function<void(bool)> m_confirmCallback;

		lua::UniqueLuaReference<LUA_TFUNCTION> m_choice;
		std::function<void(int, const std::string&)> m_choiceCallback;

		lua::UniqueLuaReference<LUA_TFUNCTION> m_prompt;
		std::function<void(bool, const std::string&)> m_promptCallback;

		lua::Thread m_messageThread;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_MESSAGE_MESSAGE_H


