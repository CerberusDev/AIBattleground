// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

class CapturePoint
{
private:
	sf::Sprite CapturePointSprite;
	sf::Vector2f Position;
	ETeam Team;

public:
	CapturePoint(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam);
	~CapturePoint();

	sf::Vector2f GetPosition() const;
	void Draw(sf::RenderWindow* Window) const;
};
