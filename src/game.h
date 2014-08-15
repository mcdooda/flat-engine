#ifndef FLAT_GAME_H
#define FLAT_GAME_H

#include <string>
#include "flat.h"

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

namespace flat
{

class Game : public Flat, public state::Agent
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

} // flat

using namespace flat;

#endif // FLAT_GAME_H


