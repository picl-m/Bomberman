#pragma once
#include "Scene.h"

class GameScene :
    public Scene
{
private:
    //Variables
    std::map<POWER_UPS, int> powerUps;
    int levelNum;
    int livesNum;
    int scoreNum;
    Level* levelPtr;


    void initVariables();
    void initTextures();
    void initKeybinds();
public:
    GameScene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes);
    virtual ~GameScene();

    //Functions
    void endScene();

    void updateLevels(const float& dt);
    void updateInput(const float& dt);
    void update(const float& dt);
    void render();
};

