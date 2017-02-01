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

Flat::Flat()
{
	SDL_Init(SDL_INIT_VIDEO);
	time = new time::Time;
	video = new video::Video;
	audio = new audio::Audio;
	input = new input::Input(video->window);
	random = new random::Random;
	lua = new lua::Lua(*this);
	ui = new sharp::ui::Ui(*this);
}

Flat::~Flat()
{
	SDL_Quit();
	FLAT_DELETE(ui);
	FLAT_DELETE(lua);
	FLAT_DELETE(random);
	FLAT_DELETE(input);
	FLAT_DELETE(audio);
	FLAT_DELETE(video);
	FLAT_DELETE(time);
}

} // flat


