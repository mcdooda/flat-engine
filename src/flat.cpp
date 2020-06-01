#include <SDL2/SDL.h>

#include "flat.h"

#include "time/time.h"
#include "video/video.h"
#include "audio/audio.h"
#include "input/input.h"
#include "random/random.h"
#include "sharp/ui/ui.h"

namespace flat
{

Flat::Flat(const std::string& luaPath, const std::string& assetsPath)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	time = std::make_unique<time::Time>();
	video = std::make_unique<video::Video>();
	audio = std::make_unique<audio::Audio>();
	input = std::make_unique<input::Input>(*this);
	random = std::make_unique<random::Random>();

	lua = std::make_unique<lua::Lua>(*this, luaPath, assetsPath);

	assetRepository = std::make_unique<tool::AssetRepository>(*this);

	ui = std::make_unique<sharp::ui::Ui>(*this);
	notify = std::make_unique<sharp::notify::Notify>(*this);
	message = std::make_unique<sharp::message::Message>(*this);
}

Flat::~Flat()
{
	SDL_Quit();

	ui = nullptr;
	notify = nullptr;
	message = nullptr;
}

} // flat


