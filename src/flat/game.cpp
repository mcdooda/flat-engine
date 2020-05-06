#include <iostream>

#include "flat/game.h"

namespace flat
{

Game::Game(const std::vector<std::string>& args) : Super("flat-engine/lua/", "flat-engine/assets/"),
	m_args(args),
	m_stop(false)
{

}

Game::~Game()
{

}

void Game::init()
{
	checkArgs();
	openWindow();
	setStates();
}

void Game::checkArgs()
{
	
}

void Game::openWindow()
{
	video->window->open(video->window->getDesktopSize() * 3.f / 4.f, false, true);
	//video->window->open(video->window->getDesktopSize(), true, true);
}

bool Game::isArg(int index)
{
	return index >= 0 && index < (int) m_args.size();
}

void Game::argCheckString(int index)
{
	if (index < 0 || index >= (int) m_args.size())
	{
		std::cerr << "Argument #" << index << " must be of type string" << std::endl;
		FLAT_BREAK();
		exit(1);
	}
}

std::string Game::argGetString(int index)
{
	return isArg(index) ? m_args[index] : "";
}

void Game::argCheckInt(int index)
{
	if (index < 0 || index >= (int) m_args.size())
	{
		std::cerr << "Argument #" << index << " must be of type int" << std::endl;
		FLAT_BREAK();
		exit(1);
	}
}

int Game::argGetInt(int index)
{
	return isArg(index) ? atoi(m_args[index].c_str()) : 0;
}

void Game::loop()
{
	bool running = true;
	while (running)
	{
		FLAT_PROFILE("Frame");
		
		time->beginFrame();

		input->pollEvents();
		
		{
			FLAT_PROFILE("Game state update");
			getStateMachine().update();
		}

		{
			FLAT_PROFILE("Lua end frame");
			message->update();
			lua->endFrame();
		}

		{
			FLAT_PROFILE("Video end frame");
			video->endFrame();
		}

		{
			FLAT_PROFILE("Audio end frame");
			audio->endFrame();
		}

		time->endFrame();

		running = !input->window->isClosed() && !m_stop;
	}
}

} // flat


