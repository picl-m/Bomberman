#pragma once
#include "Entity.h"
#include "Block.h"

enum POWER_UPS { RADIUS_UP = 0, BOMB_UP, SPEED_UP, LIVE_UP, TIME_UP, SOFTBLOCK_PASS, PENT_BOMB, LAST};

class Player :
    public Entity
{
private:
    sf::Vector2f dir;
    sf::Vector2f lastDir;
    std::map<std::string, sf::Texture>* textures;

    float animationTime;
    float animationSpeed;
    int animationFrame;

    std::vector<std::string> animLeft;
    std::vector<std::string> animRight;
    std::vector<std::string> animUp;
    std::vector<std::string> animDown;

    void initVariables(std::map<std::string, sf::Texture>* textures);
public:
    Player(float x, float y, std::map<std::string, sf::Texture>* textures);
    virtual ~Player();

    void setDirection(float x, float y);
    void setAnimation(sf::Texture* texture);

    void blockCollision(std::vector<Block*> blocks);

    void updateAnimation(const float& dt);
    void updateCollisions(std::vector<Block*> blocks, std::vector<Block*> softBlocks, std::map<POWER_UPS, int> powerUps);
};

