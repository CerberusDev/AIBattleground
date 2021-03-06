// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

class TextureManager
{
private:
	std::map<std::string, sf::Texture*> TextureMap;
	sf::Texture* DefaultTexture;

public:
	TextureManager();
	~TextureManager();

	sf::Vector2u InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName, const bool bRepeated = false);

private:
	static std::string GetTextureFilePath(const std::string& TextureName);
};
