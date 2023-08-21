#include "BombExplosion.h"

void BombExplosion::initVariables(float time)
{
	this->createdTime = time;
}

BombExplosion::BombExplosion(float x, float y, sf::Texture* texture, float time)
{
	this->initVariables(time);

	this->createSprite(texture);
	this->setPosition(x, y);
}

BombExplosion::~BombExplosion()
{
}

const float BombExplosion::getTime() const
{
	return this->createdTime;
}
