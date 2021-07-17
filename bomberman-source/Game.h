#pragma once

#include "MainMenuScene.h"

class Game
{
private:
	//Variables
	sf::RenderWindow *window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;
	
	std::stack<Scene*> scenes;

	std::map<std::string, int> keys;

	//Init functions
	void initWindow();
	void initScenes();
	void initKeys();
public:
	//Constructor / destructor
	Game();
	virtual ~Game();

	//Functions
	void updateDt();
	void updateSFMLEvents();
	void update();
	void render();
	void run();
};