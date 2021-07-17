#include "GameScene.h"

void GameScene::initVariables()
{
	this->levelNum = 1;
	this->livesNum = 0;
	this->scoreNum = 0;

	for (int i = RADIUS_UP; i != LAST; i++)
	{
		this->powerUps[static_cast<POWER_UPS>(i)] = 0;
	}
}

void GameScene::initTextures()
{
	//load textures

	sf::Texture temp;

	temp.loadFromFile("Resources/Images/Sprites/Player/playerLeft.png");
	this->textures["PLAYER_LEFT"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerLeft2.png");
	this->textures["PLAYER_LEFT_2"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerLeft3.png");
	this->textures["PLAYER_LEFT_3"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Player/playerRight.png");
	this->textures["PLAYER_RIGHT"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerRight2.png");
	this->textures["PLAYER_RIGHT_2"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerRight3.png");
	this->textures["PLAYER_RIGHT_3"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Player/playerUp.png");
	this->textures["PLAYER_UP"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerUp2.png");
	this->textures["PLAYER_UP_2"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerUp3.png");
	this->textures["PLAYER_UP_3"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Player/playerDown.png");
	this->textures["PLAYER_DOWN"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerDown2.png");
	this->textures["PLAYER_DOWN_2"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerDown3.png");
	this->textures["PLAYER_DOWN_3"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Player/playerDied.png");
	this->textures["PLAYER_DIED"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Player/playerWin.png");
	this->textures["PLAYER_WIN"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Enemy/test.png");
	this->textures["ENEMY_IDLE"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Background.png");
	this->textures["GAME_BACKGROUND"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Blocks/Solid.png");
	this->textures["BLOCK_SOLID"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Blocks/Soft.png");
	this->textures["BLOCK_SOFT"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Blocks/SoftHighlight.png");
	this->textures["BLOCK_SOFT_HIGHLIGHT"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Blocks/End.png");
	this->textures["END"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/PowerUps/Radius.png");
	this->textures["POWER_UP_RADIUS"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/Lives.png");
	this->textures["POWER_UP_LIVES"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/Bomb.png");
	this->textures["POWER_UP_BOMB"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/SoftblockPass.png");
	this->textures["POWER_UP_SOFTBLOCK_PASS"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/Speed.png");
	this->textures["POWER_UP_SPEED"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/Time.png");
	this->textures["POWER_UP_TIME"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/PowerUps/PentBomb.png");
	this->textures["POWER_UP_PENT_BOMB"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Bomb/Bomb.png");
	this->textures["BOMB"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Middle.png");
	this->textures["EXPLOSION_MIDDLE"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Horizontal.png");
	this->textures["EXPLOSION_HORIZONTAL"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Vertical.png");
	this->textures["EXPLOSION_VERTICAL"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Block.png");
	this->textures["EXPLOSION_BLOCK"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Top.png");
	this->textures["EXPLOSION_TOP"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Bottom.png");
	this->textures["EXPLOSION_BOTTOM"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Left.png");
	this->textures["EXPLOSION_LEFT"] = temp;
	temp.loadFromFile("Resources/Images/Sprites/Bomb/Explosion_Right.png");
	this->textures["EXPLOSION_RIGHT"] = temp;

	temp.loadFromFile("Resources/Images/Sprites/Missing.png");
	this->textures["MISSING"] = temp;
}

void GameScene::initKeybinds()
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

GameScene::GameScene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes)
	:Scene(window, keys, scenes)
{
	this->initVariables();
	this->initKeybinds();
	this->initTextures();
}

GameScene::~GameScene()
{
}

void GameScene::endScene()
{
	std::ifstream ifs("Config/game_save.ini");

	if (ifs.is_open())
	{
		std::string highScore;
		ifs >> highScore;

		if (std::stoi(highScore) < this->scoreNum)
		{
			std::ofstream ofs("Config/game_save.ini");
			if (ofs.is_open())
			{
				highScore = std::to_string(this->scoreNum);

				ofs << highScore;
			}
			ofs.close();
		}
	}

	ifs.close();

	this->quit = true;
}

void GameScene::updateLevels(const float& dt)
{
	if (this->levelPtr)
	{
		this->levelPtr->update(dt);

		if (this->levelPtr->getGameState() != "Alive")
		{
			if (this->levelPtr->getGameState() == "Win")
			{
				this->levelNum++;
			}
			else if (this->levelPtr->getGameState() == "Died")
			{
				if (this->powerUps.at(LIVE_UP) > 0)
				{
					this->powerUps.at(LIVE_UP)--;
					this->livesNum = this->powerUps.at(LIVE_UP);
				}
				else
				{
					this->endScene();
				}
			}
			else if (this->levelPtr->getGameState() == "Time")
			{
				if (this->livesNum > 0)
				{
					this->livesNum--;
				}
				else
				{
					this->endScene();
				}
			}

			delete this->levelPtr;
			this->levelPtr = nullptr;
		}
	}
	else
	{
		this->levelPtr = new Level(&this->keybinds, &this->powerUps, &this->levelNum, &this->livesNum, &this->scoreNum, &this->textures, (float)(this->window->getSize().x), (float)(this->window->getSize().y));
	}
}

void GameScene::updateInput(const float& dt)
{
	this->checkForQuit();
}

//Functions
void GameScene::update(const float& dt)
{
	this->updateMouse();
	this->updateInput(dt);
	this->updateLevels(dt);
}

void GameScene::render()
{
	if (this->levelPtr)
		this->levelPtr->render(this->window);
}
