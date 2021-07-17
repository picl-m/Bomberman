#pragma once
#include "Entity.h"
class BombExplosion :
    public Entity
{
private:
    float createdTime;

    void initVariables(float time);
public:
    BombExplosion(float x, float y, sf::Texture* texture, float time);
    virtual ~BombExplosion();

    const float getTime() const;
};

