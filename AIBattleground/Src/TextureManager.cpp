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
	for (auto &it : MyMap)
		if (it.second != DefaultTexture)
			delete it.second;

	delete DefaultTexture;
}

sf::Vector2u TextureManager::InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName)
{
	auto TexIt = MyMap.find(TextureName);
	sf::Texture* Tex;

	if (TexIt == MyMap.end())
	{
		Tex = new sf::Texture();

		if (Tex->loadFromFile(GetTextureFilePath(TextureName)))
		{
			MyMap[TextureName] = Tex;
		}
		else
		{
			delete Tex;
			Tex = DefaultTexture;
			MyMap[TextureName] = Tex;
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
