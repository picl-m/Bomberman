#include "Player.h"

void Player::initVariables(std::map<std::string, sf::Texture>* textures)
{
	this->movementSpeed = 75.f;
	this->dir = sf::Vector2f(1, 0);
	this->animationTime = 0.f;
	this->animationSpeed = 0.2f;
	this->textures = textures;

	this->animLeft.push_back("PLAYER_LEFT");
	this->animLeft.push_back("PLAYER_LEFT_2");
	this->animLeft.push_back("PLAYER_LEFT");
	this->animLeft.push_back("PLAYER_LEFT_3");

	this->animRight.push_back("PLAYER_RIGHT");
	this->animRight.push_back("PLAYER_RIGHT_2");
	this->animRight.push_back("PLAYER_RIGHT");
	this->animRight.push_back("PLAYER_RIGHT_3");

	this->animUp.push_back("PLAYER_UP");
	this->animUp.push_back("PLAYER_UP_2");
	this->animUp.push_back("PLAYER_UP");
	this->animUp.push_back("PLAYER_UP_3");

	this->animDown.push_back("PLAYER_DOWN");
	this->animDown.push_back("PLAYER_DOWN_2");
	this->animDown.push_back("PLAYER_DOWN");
	this->animDown.push_back("PLAYER_DOWN_3");
}

Player::Player(float x, float y, std::map<std::string, sf::Texture>* textures)
{
	this->createSprite(&textures->at("PLAYER_DOWN"));
	this->setPosition(x, y);
	this->initVariables(textures);
}

Player::~Player()
{
}

void Player::setDirection(float x, float y)
{
	this->dir = sf::Vector2f(x, y);
}

void Player::setAnimation(sf::Texture* texture)
{
	this->sprite->setTexture(*texture);
}

void Player::updateAnimation(const float& dt)
{
	if (lastDir == dir && dir != sf::Vector2f(0, 0))
	{
		this->animationTime += dt;

		if (this->animationTime >= animationSpeed)
		{
			this->animationTime -= animationSpeed;
			this->animationFrame++;
			if (this->animationFrame >= 4)
			{
				this->animationFrame = 0;
			}

			if (dir == sf::Vector2f(-1, 0))
			{
				this->sprite->setTexture(textures->at(animLeft.at(animationFrame)));
			}
			else if (dir == sf::Vector2f(1, 0))
			{
				this->sprite->setTexture(textures->at(animRight.at(animationFrame)));
			}
			else if (dir == sf::Vector2f(0, -1))
			{
				this->sprite->setTexture(textures->at(animUp.at(animationFrame)));
			}
			else if (dir == sf::Vector2f(0, 1))
			{
				this->sprite->setTexture(textures->at(animDown.at(animationFrame)));
			}
		}
	}
	else
	{
		this->animationTime = 0;
		this->animationFrame = 1;

		sf::Vector2f dirI;
		int i;
		if (dir == sf::Vector2f(0, 0))
		{
			dirI = lastDir;
			i = 0;
		}
		else
		{
			dirI = dir;
			i = 1;
		}

		if (dirI == sf::Vector2f(-1, 0))
		{
			this->sprite->setTexture(textures->at(animLeft.at(i)));
		}
		else if (dirI == sf::Vector2f(1, 0))
		{
			this->sprite->setTexture(textures->at(animRight.at(i)));
		}
		else if (dirI == sf::Vector2f(0, -1))
		{
			this->sprite->setTexture(textures->at(animUp.at(i)));
		}
		else if (dirI == sf::Vector2f(0, 1))
		{
			this->sprite->setTexture(textures->at(animDown.at(i)));
		}
	}

	this->lastDir = dir;
}

void Player::blockCollision(std::vector<Block*> blocks)
{
	for (auto block : blocks)
	{
		sf::Vector2f blockPosition = block->getPosition();
		sf::Vector2f blockHalfSize = sf::Vector2f(block->getSize().x / 2, block->getSize().y / 2);
		sf::Vector2f playerPosition = sf::Vector2f((float)(this->getPosition().x) - 3.f, (float)(this->getPosition().y) - 3.f);
		sf::Vector2f playerHalfSize = sf::Vector2f(this->getSize().x / 2, this->getSize().y / 2);

		float deltaX = blockPosition.x - playerPosition.x;
		float deltaY = blockPosition.y - playerPosition.y;

		float intersectX = abs(deltaX) - (blockHalfSize.x + playerHalfSize.x);
		float intersectY = abs(deltaY) - (blockHalfSize.y + playerHalfSize.y);

		if (intersectX < 0.f && intersectY < 0.f)
		{
			if (intersectX > intersectY)
			{
				if (deltaX > 0.f)
				{
					this->sprite->move(intersectX, 0.f);
				}
				else
				{
					this->sprite->move(-intersectX, 0.f);
				}
			}
			else
			{
				if (deltaY > 0.f)
				{
					this->sprite->move(0.f, intersectY);
				}
				else
				{
					this->sprite->move(0.f, -intersectY);
				}
			}
		}
	}
}

void Player::updateCollisions(std::vector<Block*> blocks, std::vector<Block*> softBlocks, std::map<POWER_UPS, int> powerUps)
{
	//Block collision
	this->blockCollision(blocks);

	if (powerUps.at(SOFTBLOCK_PASS) == 0)
		this->blockCollision(softBlocks);

	//Level edge collision
	if (this->sprite->getPosition().x < 40.f)
		this->sprite->setPosition(40.f, this->sprite->getPosition().y);
	else if (this->sprite->getPosition().x > 720.f - this->getSize().x)
		this->sprite->setPosition(720.f - this->getSize().x, this->sprite->getPosition().y);

	if (this->sprite->getPosition().y < 120.f)
		this->sprite->setPosition(this->sprite->getPosition().x, 120.f);
	else if (this->sprite->getPosition().y > 560.f - this->getSize().y)
		this->sprite->setPosition(this->sprite->getPosition().x, 560.f - this->getSize().y);
}
