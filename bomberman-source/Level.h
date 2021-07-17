#pragma once
#include "Player.h"
#include "Block.h"
#include "Bomb.h"
#include "BombExplosion.h"
#include "Enemy.h"

class Level
{
private:
	//Variables
	std::map<POWER_UPS, int>* powerUps;
	std::map<std::string, int>* keybinds;

	int* levelNum;
	int* livesNum;
	int* scoreNum;
	int maxTime;
	int bombRadius;
	int maxBombs;
	float lastBomb;
	float timeFrozen;
	bool gameReady;
	bool enemiesAlive;
	bool freezeLevel;
	POWER_UPS nextPowerUp;
	bool powerUpCollected;
	sf::Vector2f endPosition;
	sf::Vector2f powerUpPosition;

	std::string gameState;
	std::string nextGameState;
	
	std::map<std::string, sf::Texture>* textures;
	Player* player;
	Block* endBlock;
	Block* powerUp;
	sf::RectangleShape background;
	std::vector<Block*> blocks;
	std::vector<Block*> softBlocks;
	std::vector<Bomb*> bombs;
	std::vector<BombExplosion*> bombExplosion;
	std::vector<Enemy*> enemies;

	sf::Time gameTime;
	sf::Clock clock;
	sf::Time timeLeft;

	sf::Font font;
	sf::Text startText;
	sf::Text scoreText;
	sf::Text timeText;
	sf::Text livesText;

	//Inits
	void initFonts();
	void initVariables(std::map<std::string, int>* keybindsOr, std::map<POWER_UPS, int>* powerUpsOr, int* levelNumOr, int* livesOr, int* scoreNumOr, std::map<std::string, sf::Texture>* texturesOr);
	void generate();
	void initUI(float x, float y);
	void initPlayer();
public:
	//Constructor / Destructor
	Level(std::map<std::string, int>* keybindsOr, std::map<POWER_UPS, int>* powerUpsOr, int* levelNumOr, int* livesNumOr, int* scoreNumOr, std::map<std::string, sf::Texture>* texturesOr, float x, float y);
	virtual ~Level();

	//Functions
	const std::string getGameState() const;

	void dropBomb();
	void levelEnd(std::string cause);

	void updatePowerUps();
	void updateActionCollisions();
	void updateUI();
	void updateTime();
	void updateHiddenBlocks();
	void updateInput(const float& dt);
	void update(const float& dt);

	void renderUI(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
};