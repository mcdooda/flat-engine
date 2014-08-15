#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#include <string>
#include "engine.h"

// single header public API

// geometry
#include "geometry/linesegment.h"
#include "geometry/circle.h"
#include "geometry/rectangle.h"

// video
#include "video/pass.h"
#include "video/render.h"
#include "video/color.h"

// state
#include "state/state.h"

// thread
#include "thread/thread.h"

namespace engine
{

class Game : public Engine, public state::Agent
{
	public:
		Game(const std::vector<std::string>& args);
		virtual ~Game();
		
		virtual void checkArgs();
		
		void argCheckString(int index);
		std::string argGetString(int index);
		
		void argCheckInt(int index);
		int argGetInt(int index);
		
		/* game logic */
		void loop();
		inline void stop() { m_stop = true; }
		
		/* game launcher */
		template <class T> static int run(std::vector<std::string> args)
		{
			T* game = new T(args);
			game->loop();
			delete game;
			return 0;
		}
		
	private:
		std::vector<std::string> m_args;
		bool m_stop;
};

} // engine

using namespace engine;

#endif // ENGINE_GAME_H


