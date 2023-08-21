#include "MainMenuScene.h"

void MainMenuScene::initFonts()
{
	if (!this->font.loadFromFile("Fonts/prstartk.ttf"))
	{
		throw("COULD NOT LOAD FONT");
	}
}

void MainMenuScene::initKeybinds()
{
	//Loads keys from Config/keybinds.ini

	std::ifstream ifs("Config/keybinds.ini");

	if (ifs.is_open())
	{
		std::string bind = "";
		std::string key = "";

		while (ifs >> bind >> key)
		{
			this->keybinds[bind] = this->keys->at(key);
		}
	}

	ifs.close();
}

void MainMenuScene::initButtons()
{
	this->buttonList["playButton"] = new Button(304, 324, 152, 72, &this->font, "Play", 
		sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 40), sf::Color(0, 0, 0, 40));

	this->buttonList["quitButton"] = new Button(304, 424, 152, 72, &this->font, "Quit",
		sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 40), sf::Color(0, 0, 0, 40));
}

void MainMenuScene::initElements()
{
	this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));

	if (!this->backgroundTexture.loadFromFile("Resources/Images/MainMenu/Background.png"))
		throw "MainMenuScene: Failed to load Background.png";

	this->background.setTexture(&this->backgroundTexture);

	std::string highScoreNum;
	std::ifstream ifs("Config/game_save.ini");

	if (ifs.is_open())
	{
		ifs >> highScoreNum;
	}

	ifs.close();

	this->highScore.setFont(this->font);
	this->highScore.setString("High score:  " + highScoreNum);
	this->highScore.setFillColor(sf::Color(185, 185, 187, 255));
	this->highScore.setCharacterSize(20);
	this->highScore.setPosition(40, 550);
	this->highScore.setFillColor(sf::Color::White);
}

MainMenuScene::MainMenuScene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes)
	:Scene(window, keys, scenes)
{
	this->highScoreRefresh = 0.f;

	this->initFonts();
	this->initKeybinds();
	this->initButtons();
	this->initElements();
}

MainMenuScene::~MainMenuScene()
{
	for (auto it = this->buttonList.begin(); it != this->buttonList.end(); ++it)
		delete it->second;
}

//Functions

void MainMenuScene::endScene()
{
	
}

void MainMenuScene::updateInput(const float& dt)
{
	this->checkForQuit();
}

void MainMenuScene::updateButtons()
{
	for (auto& it : this->buttonList)
	{
		it.second->update(this->mouseView);
	}

	if (this->buttonList["playButton"]->isClicked())
		this->scenes->push(new GameScene(this->window, this->keys, this->scenes));

	if (this->buttonList["quitButton"]->isClicked())
		this->quit = true;
}

void MainMenuScene::updateHighScore(const float& dt)
{
	highScoreRefresh += dt;
	if (highScoreRefresh >= 1)
	{
		highScoreRefresh = 0.f;

		std::string highScoreNum;
		std::ifstream ifs("Config/game_save.ini");

		if (ifs.is_open())
		{
			ifs >> highScoreNum;
		}

		ifs.close();

		this->highScore.setString("High score:  " + highScoreNum);
	}
}

void MainMenuScene::update(const float& dt)
{
	this->updateMouse();
	this->updateInput(dt);

	this->updateButtons();
	this->updateHighScore(dt);
}

void MainMenuScene::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttonList)
	{
		it.second->render(target);
	}
}

void MainMenuScene::render()
{
	this->window->draw(background);

	this->window->draw(highScore);

	this->renderButtons(this->window);
}
