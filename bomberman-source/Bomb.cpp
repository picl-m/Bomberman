#include "Bomb.h"

void Bomb::initVariables(float time)
{
	this->dropTime = time;
}

Bomb::Bomb(float x, float y, sf::Texture* texture, float time)
{
	this->initVariables(time);

	this->createSprite(texture);
	this->setPosition(x, y);
}

Bomb::~Bomb()
{
}

const float Bomb::getTime() const
{
	return this->dropTime;
}

void Bomb::buildExplosion(sf::Vector2f dir, std::vector<Block*> blocks, std::vector<Block*>* softBlocks, std::vector<Bomb*> bombs, std::vector<BombExplosion*>* bombExplosion, int bombRadius
	, std::map<std::string, sf::Texture>* textures, int pentBomb, float gameTime)
{
	float x = this->sprite->getPosition().x;
	float y = this->sprite->getPosition().y;

	dir = sf::Vector2f(dir.x * 40 + x, dir.y * 40 + y);
	int tilesIterated = 0;

	while (true)
	{
		bool breakLoop = false;
		bool skip = false;

		if (dir.x > 680 || dir.x < 40 || dir.y > 520 || dir.y < 120)
			break;

		for (auto block : blocks)
		{
			if (block->getPosition() == dir)
			{
				breakLoop = true;
				break;
			}
		}

		for (auto explosion : *bombExplosion)
		{
			if (explosion->getPosition() == dir)
			{
				breakLoop = true;
				break;
			}
		}

		for (int i = 0; i != softBlocks->size(); i++) {
			if (softBlocks->at(i)->getPosition() == dir)
			{
				delete softBlocks->at(i);
				softBlocks->erase(softBlocks->begin() + i);
				BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_BLOCK"), gameTime);
				bombExplosion->push_back(explosion);
				
				if (pentBomb == 0)
				{
					breakLoop = true;
				}
				else
				{
					if (dir.x > x)
					{
						dir.x = dir.x + 40;
					}
					else if (dir.x < x)
					{
						dir.x = dir.x - 40;
					}
					else if (dir.y > y)
					{
						dir.y = dir.y + 40;
					}
					else if (dir.y < y)
					{
						dir.y = dir.y - 40;
					}
					if (tilesIterated >= bombRadius)
					{
						breakLoop = true;
					}
					else
					{
						skip = true;
					}

				}
				break;
			}
		}

		if (breakLoop == true)
			break;

		if (skip == false)
		{
			if (tilesIterated >= bombRadius)
			{
				if (dir.x > x)
				{
					BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_RIGHT"), gameTime);
					bombExplosion->push_back(explosion);
				}
				else if (dir.x < x)
				{
					BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_LEFT"), gameTime);
					bombExplosion->push_back(explosion);
				}
				else if (dir.y > y)
				{
					BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_BOTTOM"), gameTime);
					bombExplosion->push_back(explosion);
				}
				else if (dir.y < y)
				{
					BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_TOP"), gameTime);
					bombExplosion->push_back(explosion);
				}
				break;
			}

			tilesIterated++;

			if (dir.x > x)
			{
				BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_HORIZONTAL"), gameTime);
				bombExplosion->push_back(explosion);
				dir.x = dir.x + 40;
			}
			else if (dir.x < x)
			{
				BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_HORIZONTAL"), gameTime);
				bombExplosion->push_back(explosion);
				dir.x = dir.x - 40;
			}
			else if (dir.y > y)
			{
				BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_VERTICAL"), gameTime);
				bombExplosion->push_back(explosion);
				dir.y = dir.y + 40;
			}
			else if (dir.y < y)
			{
				BombExplosion* explosion = new BombExplosion(dir.x, dir.y, &textures->at("EXPLOSION_VERTICAL"), gameTime);
				bombExplosion->push_back(explosion);
				dir.y = dir.y - 40;
			}
		}
	}
}

void Bomb::detonateBomb(std::vector<Block*> blocks, std::vector<Block*>* softBlocks, std::vector<Bomb*> bombs, std::vector<BombExplosion*>* bombExplosion, int bombRadius
	, std::map<std::string, sf::Texture>* textures, int pentBomb, float gameTime)
{
	BombExplosion* explosion = new BombExplosion(this->sprite->getPosition().x, this->sprite->getPosition().y, &textures->at("EXPLOSION_MIDDLE"), gameTime);
	bombExplosion->push_back(explosion);

	buildExplosion(sf::Vector2f(1.f, 0.f), blocks, softBlocks, bombs, bombExplosion, bombRadius, textures, pentBomb, gameTime);
	buildExplosion(sf::Vector2f(-1.f, 0.f), blocks, softBlocks, bombs, bombExplosion, bombRadius, textures, pentBomb, gameTime);
	buildExplosion(sf::Vector2f(0.f, 1.f), blocks, softBlocks, bombs, bombExplosion, bombRadius, textures, pentBomb, gameTime);
	buildExplosion(sf::Vector2f(0.f, -1.f), blocks, softBlocks, bombs, bombExplosion, bombRadius, textures, pentBomb, gameTime);

	delete this;
}