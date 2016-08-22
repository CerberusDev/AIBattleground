#pragma once

#include <SFML\Graphics.hpp>

class TextureManager
{
private:
	std::map<std::string, sf::Texture*> MyMap;
	const std::string DefaultTexName;

public:
	TextureManager();
	~TextureManager();

	sf::Vector2u InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName) const;
};

