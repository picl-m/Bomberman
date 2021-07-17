#include "Enemy.h"

Enemy::Enemy(float x, float y, sf::Texture* texture)
{
	this->createSprite(texture);
	this->setPosition(x, y);
	this->dir = sf::Vector2f(0, 0);
	this->nextPos = sf::Vector2f(0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::setSpeed(float speed)
{
	this->movementSpeed = speed;
}

sf::Vector2f Enemy::randomDirection(sf::Vector2f position, bool left, bool right, bool up, bool down)
{
	std::vector<int> directions;

	if (left == true)
	{
		directions.push_back(0);
	}
	if (right == true)
	{
		directions.push_back(1);
	}
	if (up == true)
	{
		directions.push_back(2);
	}
	if (down == true)
	{
		directions.push_back(3);
	}

	int i = rand() % (directions.size());

	sf::Vector2f positionX;

	if (directions[i] == 0) //left
	{
		positionX = sf::Vector2f(position.x - 40, position.y);
	}
	else if (directions[i] == 1) //right
	{
		positionX = sf::Vector2f(position.x + 40, position.y);
	}
	else if (directions[i] == 2) //up
	{
		positionX = sf::Vector2f(position.x, position.y - 40);
	}
	else if (directions[i] == 3) //down
	{
		positionX = sf::Vector2f(position.x, position.y + 40);
	}

	return positionX;
}

void Enemy::update(const float& dt, std::vector<Block*> blocks, std::vector<Block*> softBlocks, std::vector<Bomb*> bombs)
{
	//Enemy movement

	if (this->nextPos == sf::Vector2f(0, 0))
	{
		//Aviable directions
		bool left = true;
		bool right = true;
		bool up = true;
		bool down = true;
		bool current = true;
		sf::Vector2f position = this->sprite->getPosition();

		//Check map bounds
		if (position.x <= 40)
			left = false;
		else if (position.x >= 680)
			right = false;
		if (position.y <= 120)
			up = false;
		else if (position.y >= 520)
			down = false;

		//Check surrounding blocks
		for (auto block : blocks)
		{
			sf::Vector2f blockPos = block->getPosition();

			if (sf::Vector2f(position.x - 40, position.y) == blockPos)
				left = false;
			else if (sf::Vector2f(position.x + 40, position.y) == blockPos)
				right = false;
			else if (sf::Vector2f(position.x, position.y - 40) == blockPos)
				up = false;
			else if (sf::Vector2f(position.x, position.y + 40) == blockPos)
				down = false;
		}

		for (auto block : softBlocks)
		{
			sf::Vector2f blockPos = block->getPosition();

			if (sf::Vector2f(position.x - 40, position.y) == blockPos)
				left = false;
			else if (sf::Vector2f(position.x + 40, position.y) == blockPos)
				right = false;
			else if (sf::Vector2f(position.x, position.y - 40) == blockPos)
				up = false;
			else if (sf::Vector2f(position.x, position.y + 40) == blockPos)
				down = false;
		}

		for (auto bomb : bombs)
		{
			sf::Vector2f bombPos = bomb->getPosition();

			if (sf::Vector2f(position.x - 40, position.y) == bombPos)
				left = false;
			else if (sf::Vector2f(position.x + 40, position.y) == bombPos)
				right = false;
			else if (sf::Vector2f(position.x, position.y - 40) == bombPos)
				up = false;
			else if (sf::Vector2f(position.x, position.y + 40) == bombPos)
				down = false;
		}

		//Find best direction
		if (dir == sf::Vector2f(0, 0))
		{
			//Enemy doesnt have a direction yet
			if (not (left == false && right == false && up == false && down == false))
			{
				if (left == true && right == true)
				{
					//Enemy rather chooses a longer path
					int i = rand() % 1;
					if (i == 0)
					{
						this->nextPos = sf::Vector2f(position.x - 40, position.y);
					}
					else
					{
						this->nextPos = sf::Vector2f(position.x + 40, position.y);
					}
				}
				else if (up == true && down == true)
				{
					int i = rand() % 1;
					if (i == 0)
					{
						this->nextPos = sf::Vector2f(position.x, position.y - 40);
					}
					else
					{
						this->nextPos = sf::Vector2f(position.x, position.y + 40);
					}
				}
				else
				{
					this->nextPos = this->randomDirection(position, left, right, up, down);
				}

				if (this->nextPos.x < position.x)
					dir.x = -1;
				else if (this->nextPos.x > position.x)
					dir.x = 1;
				else if (this->nextPos.y < position.y)
					dir.y = -1;
				else if (this->nextPos.y > position.y)
					dir.y = 1;
			}
		}
		else
		{
			//Enemy has a direction from the last call of this function
			if (((dir.x == -1 && left == true) || (dir.x == 1 && right == true) || (dir.y == -1 && up == true) || (dir.y == 1 && down == true)))
				//Enemy can continue in the same direction
				this->nextPos = sf::Vector2f(position.x + (dir.x * 40), position.y + (dir.y * 40));
			else
			{
				//Enemy hit a wall and needs to choose a different direction
				if (dir.y == 0)
				{
					//X
					if (up == true && down == true)
					{
						if (dir.x == -1)
						{
							this->nextPos = sf::Vector2f(position.x + 40, position.y);
						}
						else if (dir.x == 1)
						{
							this->nextPos = sf::Vector2f(position.x - 40, position.y);
						}
					}
					else if (up == true)
					{
						this->nextPos = sf::Vector2f(position.x, position.y - 40);
					}
					else if (down == true)
					{
						this->nextPos = sf::Vector2f(position.x, position.y + 40);
					}
					else if (dir.x == -1)
					{
						this->nextPos = sf::Vector2f(position.x + 40, position.y);
					}
					else if (dir.x == 1)
					{
						this->nextPos = sf::Vector2f(position.x - 40, position.y);
					}
				}
				else if (dir.x == 0)
				{
					//Y
					if (left == true && right == true)
					{
						if (dir.y == -1)
						{
							this->nextPos = sf::Vector2f(position.x, position.y + 40);
						}
						else if (dir.y == 1)
						{
							this->nextPos = sf::Vector2f(position.x, position.y - 40);
						}
					}
					else if (left == true)
					{
						this->nextPos = sf::Vector2f(position.x - 40, position.y);
					}
					else if (right == true)
					{
						this->nextPos = sf::Vector2f(position.x + 40, position.y);
					}
					else if (dir.y == -1)
					{
						this->nextPos = sf::Vector2f(position.x, position.y + 40);
					}
					else if (dir.y == 1)
					{
						this->nextPos = sf::Vector2f(position.x, position.y - 40);
					}
				}
				else
				{
					dir = sf::Vector2f(0, 0);
				}
			}
		}

		if (dir != sf::Vector2f(0, 0))
		{
			dir = sf::Vector2f(0, 0);
			if (this->nextPos.x < position.x)
				dir.x = -1;
			else if (this->nextPos.x > position.x)
				dir.x = 1;
			else if (this->nextPos.y < position.y)
				dir.y = -1;
			else if (this->nextPos.y > position.y)
				dir.y = 1;
		}
	}

	if (dir != sf::Vector2f(0, 0))
	{
		bool posEmpty = true;
		sf::Vector2f position = this->sprite->getPosition();

		if (position.x < 40)
			posEmpty = false;
		else if (position.x > 680)
			posEmpty = false;
		if (position.y < 120)
			posEmpty = false;
		else if (position.y > 520)
			posEmpty = false;

		for (auto block : blocks)
		{
			if (block->getPosition() == nextPos)
				posEmpty = false;
		}

		for (auto block : softBlocks)
		{
			if (block->getPosition() == nextPos)
				posEmpty = false;
		}

		for (auto bomb : bombs)
		{
			if (bomb->getPosition() == nextPos)
				posEmpty = false;
		}

		if (dir.x == -1 && nextPos.x >= position.x || dir.x == 1 && nextPos.x <= position.x || dir.y == -1 && nextPos.y >= position.y || dir.y == 1 && nextPos.y <= position.y)
		{
			this->sprite->setPosition(nextPos);
			this->nextPos = sf::Vector2f(0, 0);
		}
		else if (posEmpty)
			this->move(dt, dir.x, dir.y);
		else
		{


			if (dir.x == -1)
			{
				nextPos.x = nextPos.x + 40;
				dir.x = 1;
			}
			else if (dir.x == 1)
			{
				nextPos.x = nextPos.x - 40;
				dir.x = -1;
			}
			else if (dir.y == -1)
			{
				nextPos.y = nextPos.y + 40;
				dir.y = 1;
			}
			else if (dir.y == 1)
			{
				nextPos.y = nextPos.y - 40;
				dir.y = -1;
			}
		}
	}
}
