#pragma once
#include "Entity.h"
class Block :
    public Entity
{
private:


public:
    Block(float x, float y, sf::Texture* texture);
    virtual ~Block();
};

