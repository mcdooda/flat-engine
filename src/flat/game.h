#ifndef FLAT_GAME_H
#define FLAT_GAME_H

#include <string>
#include "../flat.h"
#include "../state/agent.h"

namespace flat
{

class Game : public Flat, public state::Agent
{
	using Super = Flat;
	public:
		Game(const std::vector<std::string>& args);
		~Game() override;
		
		void init();
		
		virtual void setStates() = 0;
		virtual void checkArgs();
		virtual void openWindow();
		
		bool isArg(int index);
		
		void argCheckString(int index);
		std::string argGetString(int index);
		
		void argCheckInt(int index);
		int argGetInt(int index);
		
		/* game logic */
		void loop();
		inline void stop() { m_stop = true; }
		
		/* game launcher */
		template <class GameType>
		static int run(int argc, char* argv[])
		{
			FLAT_INIT_PROFILER();
			static_assert(std::is_base_of<Game, GameType>::value, "GameType must inherit from Game");
			std::vector<std::string> args;
			args.assign(argv, argv + argc);
			GameType* game = new GameType(args);
			game->init();
			game->loop();
			delete game;
			FLAT_DEINIT_PROFILER();
			return 0;
		}
		
	private:
		std::vector<std::string> m_args;
		bool m_stop;
};

} // flat

#endif // FLAT_GAME_H


