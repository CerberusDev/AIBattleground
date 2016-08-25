// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

class Actor
{
private:
	class LevelInfo* LevelInfo;
	sf::Sprite MySprite;
	sf::Vector2f Position;
	sf::Vector2f MovementDirection;
	sf::Vector2u Size;
	float Speed;

public:
	Actor(class LevelInfo* argLevelInfo, class TextureManager* TexManager, const std::string& TexName, const sf::Vector2f& InitialPosition);
	~Actor();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime);

private:
	void GenerateRandomMovementDirection(Direction DirectionToAvoid = Direction::NONE);
};
