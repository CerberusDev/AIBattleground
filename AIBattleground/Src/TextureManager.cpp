#include <iostream>

#include "TextureManager.h"

TextureManager::TextureManager() :
DefaultTexName("DefaultTex")
{
	std::vector<std::string> TexNames;
	TexNames.push_back(DefaultTexName);

	TexNames.push_back("TestTex32a");

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

void TextureManager::InitTexture(sf::Sprite* SpriteToInit, const std::string& TextureName) const
{
	auto TexIt = MyMap.find(TextureName);

	if (TexIt == MyMap.end())
	{
		std::cout << "Can't find " << TextureName << " texture, using default one instead." << std::endl;
		SpriteToInit->setTexture(*MyMap.at(DefaultTexName));
	}
	else
	{
		SpriteToInit->setTexture(*TexIt->second);
	}
}