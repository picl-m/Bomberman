#pragma once
#include "Entity.h"
#include "BombExplosion.h"
#include "Block.h"

class Bomb :
    public Entity
{
private:
    float dropTime;

    void initVariables(float time);
public:
    Bomb(float x, float y, sf::Texture* texture, float time);
    virtual ~Bomb();

    const float getTime() const;

    void buildExplosion(sf::Vector2f dir, std::vector<Block*> blocks, std::vector<Block*>* softBlocks, std::vector<Bomb*> bombs, std::vector<BombExplosion*>* bombExplosion, int bombRadius
        , std::map<std::string, sf::Texture>* textures, int pentBomb, float gameTime);

    void detonateBomb(std::vector<Block*> blocks, std::vector<Block*>* softBlocks, std::vector<Bomb*> bombs, std::vector<BombExplosion*>* bombExplosion, int bombRadius
        , std::map<std::string, sf::Texture>* textures, int pentBomb, float gameTime);
};

