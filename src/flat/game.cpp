#include <iostream>
#include "game.h"

namespace flat
{

Game::Game(const std::vector<std::string>& args) :
	m_args(args)
{
	checkArgs();
	openWindow();
}

Game::~Game()
{
	
}

void Game::checkArgs()
{
	
}

void Game::openWindow()
{
	video->window->open(video->window->getDesktopSize() * 3.f / 4.f, false, true);
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
		time->beginFrame();
		
		input->poll();
		
		m_stop = false;
		getStateMachine()->update();
		running = !input->window->isClosed() && !m_stop;
		
		video->endFrame();
		
		time->endFrame();
	}
}

} // flat


