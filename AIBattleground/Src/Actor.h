// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

#define ROBOT_SPRITES_AMOUNT 8

class Actor
{
private:
	class LevelInfo* LevelInfo;
	Actor* NearestEnemy;
	sf::Sprite RobotSprite[ROBOT_SPRITES_AMOUNT];
	sf::Sprite LaserBeamSprite;
	sf::Sprite LaserBurstSprite;
	sf::Vector2f Position;
	sf::Vector2f DesiredMovementDirection;
	sf::Vector2f ActualMovementDirection;
	sf::Vector2u Size;
	sf::Vector2u BeamTexSize;
	sf::Vector2f VectorTowardsEnemy;
	float ShotDist;
	sf::Vector2f MovementDirectionOffset;
	float MovementStopOffset;
	float MovementSpeed;
	float DirectionChangeSpeed;
	float MaxHP;
	float HP;
	float Damage;
	ETeam Team;
	sf::Vector2f MovementDirectionInterpStart;
	float MovementDirectionInterpAlpha;
	bool bInterpolateMovementDirection;
	mutable bool bDrawLaser;
	mutable float AngleToEnemy;
	sf::Time ShotInterval;
	sf::Time ShotTimeCounter;

public:
	Actor(class LevelInfo* argLevelInfo, class TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition);
	~Actor();

	void DrawRobot(sf::RenderWindow* Window) const;
	const sf::Sprite& GetRobotSprite() const;
	void DrawLaserBurst(sf::RenderWindow* Window) const;
	void DrawLaserBeam(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime);
	void TakeDamage(float DamageAmount);
	sf::Vector2f GetPosition() const;
	ETeam GetTeam() const;
	void SetNearestEnemy(Actor* NewNearestEnemy);
	Actor* GetNearestEnemy() const;

private:
	void TryToShoot();
	void GenerateRandomMovementDirection(EDirection DirectionToAvoid = EDirection::NONE);
};
