// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

class HealZone
{
private:
	sf::Sprite HealZoneSprite;
	float Radius;
	sf::Vector2f Position;
	ETeam Team;

public:
	HealZone(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam);
	~HealZone();

	void Draw(sf::RenderWindow* Window) const;
};
