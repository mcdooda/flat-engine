#include <SDL2/SDL.h>
#include "engine.h"

#include "input/input.h"
#include "video/video.h"
#include "audio/audio.h"
#include "time/time.h"
#include "random/random.h"

namespace engine
{

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO);
	time = new time::Time;
	video = new video::Video;
	audio = new audio::Audio;
	random = new random::Random;
	input = new input::Input(video->window);
}

Engine::~Engine()
{
	SDL_Quit();
	delete time;
	delete video;
	delete audio;
	delete random;
	delete input;
}

} // engine


