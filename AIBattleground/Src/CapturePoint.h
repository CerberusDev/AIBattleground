// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

#define CAPTURE_POINT_SPRITES_NUMBER 23

class CapturePoint
{
private:
	class LevelInfo* MyLevelInfo;
	class Actor** ActorsArray;
	const int ActorsNumber;
	sf::Sprite CapturePointSprite[CAPTURE_POINT_SPRITES_NUMBER];
	sf::Vector2f Position;
	ETeam Team;
	float MaxHP;
	float HP;
	float LowHPThreshold;
	float Size;

public:
	CapturePoint(class LevelInfo* argLevelInfo, class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam);
	~CapturePoint();

	void TakeDamage(float DamageAmount);
	void Draw(sf::RenderWindow* Window) const;
	const sf::Sprite& GetCurrentSprite() const;
	sf::Vector2f GetPosition() const;
	float GetSize() const;
};
