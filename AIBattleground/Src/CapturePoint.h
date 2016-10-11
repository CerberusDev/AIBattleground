// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

#define CAPTURE_POINT_SPRITES_NUMBER 23

class CapturePoint
{
private:
	sf::Sprite CapturePointSprite[CAPTURE_POINT_SPRITES_NUMBER];
	sf::Vector2f Position;
	ETeam Team;
	float MaxHP;
	float HP;

public:
	CapturePoint(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam);
	~CapturePoint();

	sf::Vector2f GetPosition() const;
	void Draw(sf::RenderWindow* Window) const;
	const sf::Sprite& GetCurrentSprite() const;
};
