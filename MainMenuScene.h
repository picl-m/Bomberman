#pragma once
#include "GameScene.h"
#include "Button.h"

class MainMenuScene :
    public Scene
{
private:
    //Variables
    float highScoreRefresh;

    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text highScore;

    std::map<std::string, Button*> buttonList;

    //Functions
    void initFonts();
    void initKeybinds();
    void initButtons();
    void initElements();
public:
    MainMenuScene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes);
    virtual ~MainMenuScene();

    //Functions
    void endScene();

    void updateInput(const float& dt);
    void updateButtons();
    void updateHighScore(const float& dt);
    void update(const float& dt);

    void renderButtons(sf::RenderTarget* target);
    void render();
};

