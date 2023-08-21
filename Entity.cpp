#include "Entity.h"

void Entity::initVariables()
{
	this->texture = nullptr;
	this->sprite = nullptr;
	this->movementSpeed = 100.f;
}

Entity::Entity()
{
	this->initVariables();
}

Entity::~Entity()
{
	delete this->sprite;
}

const sf::Vector2f Entity::getSize() const
{
	return sf::Vector2f(this->sprite->getTextureRect().width, this->sprite->getTextureRect().height);
}

const sf::Vector2f Entity::getPosition() const
{
	return this->sprite->getPosition();
}

const sf::FloatRect Entity::getBounds() const
{
	return this->sprite->getGlobalBounds(); 
}




void Entity::createSprite(sf::Texture* texture)
{
	this->texture = texture;
	this->sprite = new sf::Sprite(*this->texture);
}



void Entity::setPosition(const float x, const float y)
{
	if (this->sprite)
		this->sprite->setPosition(x, y);
}

void Entity::setMovementSpeed(const float speed)
{
	this->movementSpeed = speed;
}

void Entity::move(const float& dt, const float dir_x, const float dir_y)
{
	if (this->sprite)
		this->sprite->move(dir_x * this->movementSpeed * dt, dir_y * this->movementSpeed * dt);
}

void Entity::update(const float& dt)
{
	
}

void Entity::render(sf::RenderWindow* window)
{
	if (this->sprite)
		window->draw(*this->sprite);
}
