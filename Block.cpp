#include "Block.h"

Block::Block(float x, float y, sf::Texture* texture)
{
	this->createSprite(texture);
	this->setPosition(x, y);
}

Block::~Block()
{
}
