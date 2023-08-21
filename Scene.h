#pragma once
#include "Level.h"

class Scene
{
private:

protected:
	std::stack<Scene*>* scenes;
	sf::RenderWindow* window;
	std::map<std::string, int>* keys;
	std::map<std::string, int> keybinds;
	bool quit;

	sf::Vector2i mouseScreen;
	sf::Vector2i mouseWindow;
	sf::Vector2f mouseView;

	//Resources
	std::map<std::string, sf::Texture> textures;

	virtual void initKeybinds() = 0;
public:
	Scene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes);
	virtual ~Scene();

	const bool& getQuit() const;

	virtual void checkForQuit();
	virtual void endScene() = 0;

	virtual void updateMouse();
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render() = 0;
};

