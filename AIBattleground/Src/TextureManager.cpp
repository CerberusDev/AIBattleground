// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "TextureManager.h"

TextureManager::TextureManager()
{
	DefaultTexture = new sf::Texture();
	DefaultTexture->loadFromFile(GetTextureFilePath("DefaultTex"));
}

TextureManager::~TextureManager()
{
	for (auto &it : TextureMap)
		if (it.second != DefaultTexture)
			delete it.second;

	delete DefaultTexture;
}

sf::Vector2u TextureManager::InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName, const bool bRepeated)
{
	auto TexIt = TextureMap.find(TextureName);
	sf::Texture* Tex;

	if (TexIt == TextureMap.end())
	{
		Tex = new sf::Texture();

		if (Tex->loadFromFile(GetTextureFilePath(TextureName)))
		{
			TextureMap[TextureName] = Tex;
			Tex->setRepeated(bRepeated);
		}
		else
		{
			delete Tex;
			Tex = DefaultTexture;
			TextureMap[TextureName] = Tex;
		}
	}
	else
	{
		Tex = TexIt->second;
	}

	SpriteToInit->setTexture(*Tex);
	return Tex->getSize();
}

std::string TextureManager::GetTextureFilePath(const std::string& TextureName)
{
	return std::string("Tex/" + TextureName + ".png");
}
