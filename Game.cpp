#include "Game.h"

//Init functions
void Game::initWindow()
{
	//Creates window from /Config/window.ini

	std::ifstream ifs("Config/window.ini");

	std::string title = "BOMBERMAN";
	sf::VideoMode windowBounds(760, 600);
	unsigned framerateLimit = 60;
	bool verticalSync = false;

	if (ifs.is_open())
	{
		ifs >> framerateLimit;
		ifs >> verticalSync;
	}

	ifs.close();

	this->window = new sf::RenderWindow(windowBounds, title, sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(framerateLimit);
	this->window->setVerticalSyncEnabled(verticalSync);
}

void Game::initKeys()
{
	//Load keys

	std::ifstream ifs("Config/key_list.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		int i = 0;

		while (ifs >> key)
		{
			this->keys[key] = i;
			i++;
		}
	}

	ifs.close();
}

void Game::initScenes()
{
	this->scenes.push(new MainMenuScene(this->window, &this->keys, &this->scenes));
}

//Constructor / destructor
Game::Game()
{
	this->initWindow();
	this->initKeys();
	this->initScenes();
}

Game::~Game()
{
	delete this->window;

	//Delete all scenes
	while (!this->scenes.empty())
	{
		delete this->scenes.top();
		this->scenes.pop();
	}
}

//Functions

void Game::updateDt()
{
	//Updating the delta time
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == sf::Event::Closed)
			this->window->close();
	}
}

void Game::update()
{
	this->updateSFMLEvents();

	if (!this->scenes.empty())
	{
		this->scenes.top()->update(this->dt);

		if (this->scenes.top()->getQuit())
		{
			this->scenes.top()->endScene();
			delete this->scenes.top();
			this->scenes.pop();
		}
	}
	else
	{
		this->window->close();
	}
}

void Game::render()
{
	this->window->clear();

	if (!this->scenes.empty())
		this->scenes.top()->render();

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}
