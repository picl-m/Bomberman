#include "Scene.h"

Scene::Scene(sf::RenderWindow* window, std::map<std::string, int>* keys, std::stack<Scene*>* scenes)
{
	this->window = window;
	this->keys = keys;
	this->scenes = scenes;
	this->quit = false;
}

Scene::~Scene()
{

}

void Scene::checkForQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->quit = true;
	}
}

void Scene::updateMouse()
{
	this->mouseScreen = sf::Mouse::getPosition();
	this->mouseWindow = sf::Mouse::getPosition(*this->window);
	this->mouseView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

const bool& Scene::getQuit() const
{
	return this->quit;
}
