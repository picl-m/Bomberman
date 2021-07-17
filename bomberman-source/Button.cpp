#include "Button.h"

Button::Button(float x, float y, float width, float height, sf::Font* font, std::string text, sf::Color idleColor, sf::Color hoverColor, sf::Color clickedColor)
{
	this->buttonState = BTN_IDLE;
	this->wasClicked = 0;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color(185, 185, 187, 255));
	this->text.setCharacterSize(20);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);

	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->clickedColor = clickedColor;

	this->shape.setFillColor(this->idleColor);
}

Button::~Button()
{
}

const bool Button::isClicked() const
{
	if (this->buttonState != BTN_CLICKED && this->wasClicked != 0)
		return true;

	return false;
}

void Button::update(const sf::Vector2f mouse)
{
	this->buttonState = BTN_IDLE;

	if (this->shape.getGlobalBounds().contains(mouse))
	{
		this->buttonState = BTN_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->buttonState = BTN_CLICKED;
	}
	
	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(idleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(hoverColor);
		break;
	case BTN_CLICKED:
		this->shape.setFillColor(clickedColor);
		break;
	default:

		break;
	}

	if (this->wasClicked == 1 && this->buttonState != BTN_CLICKED)
		this->wasClicked = 2;
	else if (this->buttonState == BTN_CLICKED)
		this->wasClicked = 1;
	else if (this->wasClicked == 2)
	{
		this->wasClicked = 0;
	}
}

void Button::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
	target->draw(this->text);
}
