#include <SDL2/SDL.h>
#include "flat.h"

#include "input/input.h"
#include "video/video.h"
#include "audio/audio.h"
#include "time/time.h"
#include "random/random.h"

namespace flat
{

Flat::Flat()
{
	SDL_Init(SDL_INIT_VIDEO);
	time = new time::Time;
	video = new video::Video;
	audio = new audio::Audio;
	random = new random::Random;
	input = new input::Input(video->window);
}

Flat::~Flat()
{
	SDL_Quit();
	FLAT_DELETE(time);
	FLAT_DELETE(video);
	FLAT_DELETE(audio);
	FLAT_DELETE(random);
	FLAT_DELETE(input);
}

} // flat


