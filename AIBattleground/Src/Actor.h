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
	sf::Sprite RobotSprite;
	sf::Sprite BeamSprite;
	sf::Vector2f Position;
	sf::Vector2f DesiredMovementDirection;
	sf::Vector2f ActualMovementDirection;
	sf::Vector2u Size;
	sf::Vector2u BeamTexSize;
	sf::Vector2f VectorTowardsEnemy;
	float MovementSpeed;
	float DirectionChangeSpeed;
	ETeam Team;
	sf::Vector2f MovementDirectionInterpStart;
	float MovementDirectionInterpAlpha;
	bool bInterpolateMovementDirection;
	mutable bool bDrawBeam;
	sf::Time ShotInterval;
	sf::Time ShotTimeCounter;

public:
	Actor(class LevelInfo* argLevelInfo, class TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition);
	~Actor();

	void DrawRobot(sf::RenderWindow* Window) const;
	void DrawBeam(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime);
	sf::Vector2f GetPosition() const;
	ETeam GetTeam() const;
	void SetNearestEnemy(Actor* NewNearestEnemy);
	Actor* GetNearestEnemy() const;

private:
	void TryToShoot();
	void GenerateRandomMovementDirection(EDirection DirectionToAvoid = EDirection::NONE);
};
