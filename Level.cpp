#include "Level.h"

//Inits

void Level::initFonts()
{
	if (!this->font.loadFromFile("Fonts/prstartk.ttf"))
	{
		throw("COULD NOT LOAD FONT");
	}
}

void Level::initVariables(std::map<std::string, int>* keybindsOr, std::map<POWER_UPS, int>* powerUpsOr, int* levelNumOr, int* livesOr, int* scoreNumOr, std::map<std::string, sf::Texture>* texturesOr)
{
	this->keybinds = keybindsOr;

	this->powerUps = powerUpsOr;
	this->levelNum = levelNumOr;
	this->livesNum = livesOr;
	this->scoreNum = scoreNumOr;
	this->maxTime = 300 - ((*this->levelNum - 1) * 15) + (powerUps->at(TIME_UP) * 30);
	this->bombRadius = powerUps->at(RADIUS_UP);
	this->maxBombs = powerUps->at(BOMB_UP) + 1;

	this->textures = texturesOr;
	this->lastBomb = 0;
	this->timeFrozen = 0.f;
	this->gameReady = false;
	this->enemiesAlive = true;
	this->powerUpCollected = false;
	this->freezeLevel = false;

	this->gameState = "Alive";
	this->nextGameState = "Alive";
}

void Level::initUI(float x, float y)
{
	this->background.setSize(sf::Vector2f(x, y));
	this->background.setTexture(&this->textures->at("GAME_BACKGROUND"));

	this->startText.setFont(this->font);
	this->startText.setString("Level " + std::to_string(*this->levelNum));
	this->startText.setFillColor(sf::Color::White);
	this->startText.setCharacterSize(30);
	sf::FloatRect textRect = startText.getLocalBounds();
	this->startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->startText.setPosition(sf::Vector2f(400 , 300));

	this->scoreText.setFont(this->font);
	this->scoreText.setString(std::to_string(*this->scoreNum));
	this->scoreText.setFillColor(sf::Color::White);
	this->scoreText.setCharacterSize(27);
	this->scoreText.setPosition(sf::Vector2f(87, 25));

	this->timeText.setFont(this->font);
	this->timeText.setString("0:00");
	this->timeText.setFillColor(sf::Color::White);
	this->timeText.setCharacterSize(27);
	this->timeText.setPosition(sf::Vector2f(467, 25));

	this->livesText.setFont(this->font);
	this->livesText.setString(std::to_string(*this->livesNum));
	this->livesText.setFillColor(sf::Color::White);
	this->livesText.setCharacterSize(27);
	this->livesText.setPosition(sf::Vector2f(647, 25));
}

void Level::generate()
{
	//Generate solid blocks
	float y = 160.f;
	while (y <= 480.f)
	{
		float x = 80.f;
		while (x <= 680.f)
		{
			Block* block = new Block(x, y, &this->textures->at("BLOCK_SOLID"));
			this->blocks.push_back(block);

			x = x + 80.f;
		}

		y = y + 80.f;
	}

	//Generate soft blocks
	Block* block = new Block(120, 120, &this->textures->at("BLOCK_SOFT"));
	this->softBlocks.push_back(block);
	block = new Block(40, 200, &this->textures->at("BLOCK_SOFT"));
	this->softBlocks.push_back(block);

	while (softBlocks.size() != 20 + (*this->levelNum * 5))
	{
		float x = (float)(40*(round((40 + (std::rand() % (719 - 40 + 1))) / 40)));
		float y = (float)(40*(round((120 + (std::rand() % (559 - 120 + 1))) / 40)));
		
		bool taken = false;

		for (auto block: blocks)
		{
			if (block->getPosition() == sf::Vector2f(x, y))
			{
				taken = true;
			}
		}
		for (auto block : softBlocks)
		{
			if (block->getPosition() == sf::Vector2f(x, y))
			{
				taken = true;
			}
		}
		
		if (sf::Vector2f(x, y) == sf::Vector2f(40, 120) || sf::Vector2f(x, y) == sf::Vector2f(80, 120) || sf::Vector2f(x, y) == sf::Vector2f(40, 160))
			taken = true;

		if (taken == false)
		{
			Block* block = new Block(x, y, &this->textures->at("BLOCK_SOFT"));
			this->softBlocks.push_back(block);
		}
	}

	//Generate enemies
	while (enemies.size() != 2 + *this->levelNum)
	{
		float x = (float)(40 * (round((40 + (std::rand() % (719 - 40 + 1))) / 40)));
		float y = (float)(40 * (round((120 + (std::rand() % (559 - 120 + 1))) / 40)));

		bool taken = false;

		for (auto block : blocks)
		{
			if (block->getPosition() == sf::Vector2f(x, y))
			{
				taken = true;
			}
		}
		for (auto block : softBlocks)
		{
			if (block->getPosition() == sf::Vector2f(x, y))
			{
				taken = true;
			}
		}
		for (auto enemy : enemies)
		{
			if (enemy->getPosition() == sf::Vector2f(x, y))
			{
				taken = true;
			}
		}

		

		if (sf::Vector2f(x, y) == sf::Vector2f(40, 120) || sf::Vector2f(x, y) == sf::Vector2f(80, 120) || sf::Vector2f(x, y) == sf::Vector2f(40, 160))
			taken = true;

		if (taken == false)
		{
			Enemy* enemy = new Enemy(x, y, &this->textures->at("ENEMY_IDLE"));
			this->enemies.push_back(enemy);
			enemy->setSpeed(50.f);
		}
	}

	//Generate hidden blocks position
	int blockIndex = std::rand() % (this->softBlocks.size());
	this->endPosition = this->softBlocks[blockIndex]->getPosition();

	while (powerUpPosition == sf::Vector2f(0, 0))
	{
		int blockIndex = std::rand() % (this->softBlocks.size());
		if (this->softBlocks[blockIndex]->getPosition() != endPosition)
		{
			this->powerUpPosition = this->softBlocks[blockIndex]->getPosition();
		}
	}

	//Generate power up
	//{ RADIUS_UP = 0, BOMB_UP, SPEED_UP, LIVE_UP, TIME_UP, NO_COLLISION, LAST};
	std::vector<POWER_UPS> aviablePowerUps;

	if (this->powerUps->at(RADIUS_UP) <= 3)
	{
		aviablePowerUps.push_back(RADIUS_UP);
	}
	if (this->powerUps->at(LIVE_UP) <= 2)
	{
		aviablePowerUps.push_back(LIVE_UP);
	}
	if (this->powerUps->at(BOMB_UP) <= 1)
	{
		aviablePowerUps.push_back(BOMB_UP);
	}
	if (this->powerUps->at(SOFTBLOCK_PASS) == 0 && *this->levelNum >= 5)
	{
		aviablePowerUps.push_back(SOFTBLOCK_PASS);
	}
	if (this->powerUps->at(SPEED_UP) == 0 && *this->levelNum >= 5)
	{
		aviablePowerUps.push_back(SPEED_UP);
	}
	if (this->powerUps->at(PENT_BOMB) == 0 && *this->levelNum >= 5)
	{
		aviablePowerUps.push_back(PENT_BOMB);
	}
	if (*this->levelNum >= 5)
	{
		aviablePowerUps.push_back(TIME_UP);
	}

	int i = rand() % aviablePowerUps.size();
	this->nextPowerUp = aviablePowerUps[i];
}

void Level::initPlayer()
{
	this->player = new Player(40, 120, this->textures);
	if (this->powerUps->at(SPEED_UP) != 0)
	{
		this->player->setMovementSpeed(125.f);
	}
}

//Constructor/Destructor

Level::Level(std::map<std::string, int>* keybindsOr, std::map<POWER_UPS, int>* powerUpsOr, int* levelNumOr, int* livesNumOr, int* scoreNumOr, std::map<std::string, sf::Texture>* texturesOr, float x, float y)
{
	this->initFonts();
	this->initVariables(keybindsOr, powerUpsOr, levelNumOr, livesNumOr, scoreNumOr, texturesOr);
	this->initUI(x, y);
	this->generate();
	this->initPlayer();
	this->clock.restart();
}

Level::~Level()
{
	for (auto x : blocks)
	{
		delete x;
	}
	for (auto x : softBlocks)
	{
		delete x;
	}
	for (auto x : bombs)
	{
		delete x;
	}
	for (auto x : bombExplosion)
	{
		delete x;
	}
	delete this->player;
	delete this->endBlock;
}

//Functions

const std::string Level::getGameState() const
{
	return gameState;
}

void Level::dropBomb()
{
	float x = 40 * (round(this->player->getPosition().x / 40));
	float y = 40 * (round(this->player->getPosition().y / 40));

	bool taken = false;
	for (auto bomb : bombs)
	{
		if (bomb->getPosition() == sf::Vector2f(x, y))
			taken = true;
	}

	for (auto block : softBlocks)
	{
		if (block->getPosition() == sf::Vector2f(x, y))
			taken = true;
	}

	if (taken == false)
	{
		Bomb* bomb = new Bomb(x, y, &this->textures->at("BOMB"), this->gameTime.asSeconds());
		this->bombs.push_back(bomb);
	}
}

void Level::levelEnd(std::string cause)
{
	//States: Alive, Win, Died, Time
	this->gameState = cause;
}

void Level::updatePowerUps()
{
	if (enemiesAlive == true && this->enemies.size() == 0)
	{
		//Highlight soft block with power up under it if all enemies have been killed
		enemiesAlive = false;
		for (int i = 0; i != this->softBlocks.size(); i++) {
			if (softBlocks[i]->getPosition() == powerUpPosition)
			{
				delete softBlocks[i];
				this->softBlocks.erase(this->softBlocks.begin() + i);

				Block* block = new Block(powerUpPosition.x, powerUpPosition.y, &this->textures->at("BLOCK_SOFT_HIGHLIGHT"));
				this->softBlocks.push_back(block);

				break;
			}
		}
	}
}

void Level::updateActionCollisions()
{
	//Update collisions with interactive objects

	sf::FloatRect playerBounds = this->player->getBounds();

	if (this->endBlock)
	{
		sf::Vector2f halfSize = sf::Vector2f(this->player->getSize().x / 2, this->player->getSize().y / 2);
		if (this->player->getPosition().x + halfSize.x > endPosition.x && this->player->getPosition().y + halfSize.y > endPosition.y 
			&& this->player->getPosition().x + halfSize.x < endPosition.x + endBlock->getSize().x && this->player->getPosition().y + halfSize.y < endPosition.y + endBlock->getSize().y)
		{
			this->freezeLevel = true;
			this->timeFrozen = gameTime.asSeconds();
			this->nextGameState = "Win";
			this->player->setAnimation(&this->textures->at("PLAYER_WIN"));
		}
	}

	if (this->powerUp && this->powerUp->getBounds().intersects(playerBounds))
	{
		*this->scoreNum += 1000;

		this->powerUps->at(nextPowerUp)++;

		delete this->powerUp;
		this->powerUp = nullptr;
		this->powerUpCollected = true;

		switch (nextPowerUp)
		{
		case RADIUS_UP:
			this->bombRadius = this->powerUps->at(RADIUS_UP);
			break;
		case BOMB_UP:
			this->maxBombs = this->powerUps->at(BOMB_UP) + 1;
			break;
		case LIVE_UP:
			*this->livesNum = this->powerUps->at(LIVE_UP);
			break;
		case SPEED_UP:
			this->player->setMovementSpeed(125.f);
			break;
		case TIME_UP:
			this->maxTime = this->maxTime + 30;
			break;
		}
	}

	for (auto explosion : bombExplosion)
	{
		if (explosion->getBounds().intersects(playerBounds))
		{
			this->freezeLevel = true;
			this->timeFrozen = gameTime.asSeconds();
			this->nextGameState = "Died";
			this->player->setAnimation(&this->textures->at("PLAYER_DIED"));
		}

		for (int i = 0; i != this->enemies.size(); i++) {
			if (explosion->getBounds().intersects(this->enemies[i]->getBounds()))
			{
				delete enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				*this->scoreNum += 100;
				i--;
			}
		}
	}

	for (auto enemy : enemies)
	{
		if (enemy->getBounds().intersects(playerBounds))
		{
			this->freezeLevel = true;
			this->timeFrozen = gameTime.asSeconds();
			this->nextGameState = "Died";
			this->player->setAnimation(&this->textures->at("PLAYER_DIED"));
		}
	}
}

void Level::updateUI()
{
	this->scoreText.setString(std::to_string(*this->scoreNum));
	this->livesText.setString(std::to_string(*this->livesNum));

	//Timer
	if (this->maxTime - this->gameTime.asSeconds() <= 0)
	{
		this->timeText.setString("0:00");
	}
	else
	{
		int minutes = (int)(floor((this->maxTime - this->gameTime.asSeconds()) / 60));
		int seconds = (int)((this->maxTime - this->gameTime.asSeconds()) - (minutes * 60));
		if (seconds >= 10)
			this->timeText.setString(std::to_string(minutes) + ":" + std::to_string(seconds));
		else
			this->timeText.setString(std::to_string(minutes) + ":0" + std::to_string(seconds));
	}
	if (this->maxTime - this->gameTime.asSeconds() < 21)
		this->timeText.setFillColor(sf::Color::Red);
}

void Level::updateTime()
{
	//Updates time and every time dependent function

	this->gameTime = clock.getElapsedTime();

	if (this->gameReady == false && this->gameTime.asSeconds() > 3)
	{
		this->gameTime = this->clock.restart();
		this->gameReady = true;
	}

	if (this->freezeLevel == true && this->timeFrozen + 1 < this->gameTime.asSeconds())
	{
		this->levelEnd(nextGameState);
	}
	
	for (int i = 0; i != bombs.size(); i++) {
		if (bombs[i]->getTime() + 2.f < this->gameTime.asSeconds())
		{
			bombs[i]->detonateBomb(this->blocks, &this->softBlocks, this->bombs, &this->bombExplosion, this->bombRadius, this->textures, this->powerUps->at(PENT_BOMB), this->gameTime.asSeconds());
			this->bombs.erase(bombs.begin() + i);
			i--;
		}
	}

	for (int i = 0; i != bombExplosion.size(); i++) {
		if (bombExplosion[i]->getTime() + 1.f < this->gameTime.asSeconds())
		{
			delete this->bombExplosion[i];
			this->bombExplosion.erase(bombExplosion.begin() + i);
			i--;
		}
	}

	if (this->maxTime - this->gameTime.asSeconds() <= 0)
	{
		this->freezeLevel = true;
		this->timeFrozen = gameTime.asSeconds();
		this->player->setAnimation(&this->textures->at("PLAYER_DIED"));
	}
}

void Level::updateHiddenBlocks()
{
	if (endBlock == nullptr)
	{
		bool hidden = false;
		for (auto block : this->softBlocks)
		{
			if (block->getPosition() == this->endPosition)
				hidden = true;
		}

		if (hidden == false)
		{
			sf::Texture* texture = &this->textures->at("END");
			this->endBlock = new Block(endPosition.x, endPosition.y, texture);
		}
	}

	if (powerUp == nullptr && powerUpCollected == false)
	{
		bool hidden = false;
		for (auto block : this->softBlocks)
		{
			if (block->getPosition() == this->powerUpPosition)
				hidden = true;
		}

		if (hidden == false)
		{
			sf::Texture* texture;
			if (nextPowerUp == RADIUS_UP)
			{
				texture = &this->textures->at("POWER_UP_RADIUS");
			}
			else if (nextPowerUp == LIVE_UP)
			{
				texture = &this->textures->at("POWER_UP_LIVES");
			}
			else if (nextPowerUp == BOMB_UP)
			{
				texture = &this->textures->at("POWER_UP_BOMB");
			}
			else if (nextPowerUp == SOFTBLOCK_PASS)
			{
				texture = &this->textures->at("POWER_UP_SOFTBLOCK_PASS");
			}
			else if (nextPowerUp == SPEED_UP)
			{
				texture = &this->textures->at("POWER_UP_SPEED");
			}
			else if (nextPowerUp == TIME_UP)
			{
				texture = &this->textures->at("POWER_UP_TIME");
			}
			else if (nextPowerUp == PENT_BOMB)
			{
				texture = &this->textures->at("POWER_UP_PENT_BOMB");
			}
			else
			{
				texture = &this->textures->at("MISSING");
			}

			this->powerUp = new Block(powerUpPosition.x, powerUpPosition.y, texture);
		}
	}
}

void Level::updateInput(const float& dt)
{
	this->player->setDirection(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds->at("MOVE_UP"))))
	{
		this->player->move(dt, 0.f, -1.f);
		this->player->setDirection(0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds->at("MOVE_LEFT"))))
	{
		this->player->move(dt, -1.f, 0.f);
		this->player->setDirection(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds->at("MOVE_DOWN"))))
	{
		this->player->move(dt, 0.f, 1.f);
		this->player->setDirection(0.f, 1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds->at("MOVE_RIGHT"))))
	{
		this->player->move(dt, 1.f, 0.f);
		this->player->setDirection(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds->at("DROP_BOMB"))))
	{
		if (this->bombs.size() < this->maxBombs && this->lastBomb + 0.25 < this->gameTime.asSeconds())
		{
			this->dropBomb();
			this->lastBomb = this->gameTime.asSeconds();
		}
	}
}

void Level::update(const float& dt)
{
	this->updateTime();

	if (this->gameReady && not this->freezeLevel)
	{
		this->updateUI();

		this->updatePowerUps();

		this->updateInput(dt);

		this->updateHiddenBlocks(); 
		
		for (auto x : enemies)
		{
			x->update(dt, blocks, softBlocks, bombs);
		}

		this->player->updateCollisions(blocks, softBlocks, *powerUps);
		this->player->updateAnimation(dt);

		this->updateActionCollisions();
	}
}



void Level::renderUI(sf::RenderWindow* window)
{
	window->draw(this->background);
	window->draw(this->scoreText);
	window->draw(this->timeText);
	window->draw(this->livesText);
}

void Level::render(sf::RenderWindow* window)
{
	if (this->gameReady)
	{
		this->renderUI(window);
		for (auto x : blocks)
		{
			x->render(window);
		}
		for (auto x : softBlocks)
		{
			x->render(window);
		}

		if (this->endBlock)
			this->endBlock->render(window);

		if (this->powerUp)
			this->powerUp->render(window);

		for (auto x : bombs)
		{
			x->render(window);
		}
		for (auto x : bombExplosion)
		{
			x->render(window);
		}

		for (auto x : enemies)
		{
			x->render(window);
		}
		this->player->render(window);
	}
	else
	{
		window->draw(startText);
	}
}
