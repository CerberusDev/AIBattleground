// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "TextureManager.h"

TextureManager::TextureManager() :
DefaultTexName("DefaultTex")
{
	std::vector<std::string> TexNames;
	TexNames.push_back(DefaultTexName);

	TexNames.push_back("TestTex16a");

	for (auto &it : TexNames)
	{
		MyMap[it] = new sf::Texture();
		MyMap[it]->loadFromFile("Tex/" + it + ".png");
	}
}

TextureManager::~TextureManager()
{
	for (auto &it : MyMap)
		delete it.second;
}

sf::Vector2u TextureManager::InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName) const
{
	auto TexIt = MyMap.find(TextureName);
	sf::Texture* Tex;

	if (TexIt == MyMap.end())
	{
		std::cout << "Can't find " << TextureName << " texture, using default one instead." << std::endl;
		Tex = MyMap.at(DefaultTexName);
	}
	else
	{
		Tex = TexIt->second;
	}

	SpriteToInit->setTexture(*Tex);
	return Tex->getSize();
}
