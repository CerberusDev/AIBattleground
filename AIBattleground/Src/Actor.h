#pragma once

#include <SFML\Graphics.hpp>

class Actor
{
private:
	sf::Sprite MySprite;
	sf::Vector2u Size;
	sf::Transform Transform;

public:
	Actor(class TextureManager* TexManager, const std::string& TexName, const sf::Vector2f& InitialPosition);
	~Actor();

	void Draw(sf::RenderWindow* Window) const;
};

