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
	float SquaredRadius;
	sf::Vector2f Position;
	ETeam Team;
	sf::Time HealTimeCounter;
	sf::Time HealTimeStep;
	float HPToHeal;
	class Actor** ActorsArray;
	int ActorsNumber;

public:
	HealZone(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam, class Actor** argActorsArray, int argActorsNumber);
	~HealZone();

	sf::Vector2f GetPosition() const;
	void Update(const float DeltaTime);
	void Draw(sf::RenderWindow* Window) const;
};
