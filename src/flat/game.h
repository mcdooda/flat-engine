#ifndef FLAT_GAME_H
#define FLAT_GAME_H

#include <string>
#include "../flat.h"

namespace flat
{

class Game : public Flat, public state::Agent
{
	public:
		Game(const std::vector<std::string>& args);
		virtual ~Game();
		
		void checkArgs();
		void openWindow();
		
		bool isArg(int index);
		
		void argCheckString(int index);
		std::string argGetString(int index);
		
		void argCheckInt(int index);
		int argGetInt(int index);
		
		/* game logic */
		void loop();
		inline void stop() { m_stop = true; }
		
		/* game launcher */
		template <class T> static int run(int argc, char* argv[])
		{
			std::vector<std::string> args;
			args.assign(argv, argv + argc);
			T* game = new T(args);
			game->loop();
			delete game;
			return 0;
		}
		
	private:
		std::vector<std::string> m_args;
		bool m_stop;
};

} // flat

#endif // FLAT_GAME_H


