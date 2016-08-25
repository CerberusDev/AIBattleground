#pragma once

#include <SFML\Graphics.hpp>

class LevelInfo
{
public:
	const sf::FloatRect Boundaries;
	const sf::Vector2f RightBottomEdge;

public:
	LevelInfo(const sf::FloatRect& LevelBoundaries);
	~LevelInfo();
};

