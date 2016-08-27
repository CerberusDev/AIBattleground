// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

class Actor
{
private:
	class LevelInfo* LevelInfo;
	Actor* NearestEnemy;
	sf::Sprite MySprite;
	sf::Vector2f Position;
	sf::Vector2f MovementDirection;
	sf::Vector2u Size;
	float Speed;
	ETeam Team;

public:
	Actor(class LevelInfo* argLevelInfo, class TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition);
	~Actor();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime);
	sf::Vector2f GetPosition() const;
	ETeam GetTeam() const;
	void SetNearestEnemy(Actor* NewNearestEnemy);

private:
	void GenerateRandomMovementDirection(Direction DirectionToAvoid = Direction::NONE);
};
