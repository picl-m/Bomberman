#pragma once
#include "Entity.h"
#include "Block.h"
#include "Bomb.h"

class Enemy :
    public Entity
{
private:
    sf::Vector2f dir; //Direction of movement
    sf::Vector2f nextPos; //Next tile the enemy will move to
public:
    Enemy(float x, float y, sf::Texture* texture);
    virtual ~Enemy();

    void setSpeed(float speed);

    sf::Vector2f randomDirection(sf::Vector2f position, bool left, bool right, bool up, bool down);
    void update(const float& dt, std::vector<Block*> blocks, std::vector<Block*> softBlocks, std::vector<Bomb*> bombs);
};