#include <SDL2/SDL.h>
#include "flat.h"

namespace flat
{

Flat::Flat(const std::string& luaPath, const std::string& assetsPath)
{
	SDL_Init(SDL_INIT_VIDEO);

	time = std::make_unique<time::Time>();
	threads = std::make_unique<threads::Threads>();
	video = std::make_unique<video::Video>();
	audio = std::make_unique<audio::Audio>();
	input = std::make_unique<input::Input>(*this);
	random = std::make_unique<random::Random>();
	lua = std::make_unique<lua::Lua>(*this, luaPath, assetsPath);
	ui = std::make_unique<sharp::ui::Ui>(*this);
}

Flat::~Flat()
{
	SDL_Quit();
}

} // flat


