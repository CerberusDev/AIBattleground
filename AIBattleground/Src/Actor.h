// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <atomic>

#include "Globals.h"
#include "Blackboard.h"

#define ROBOT_SPRITES_NUMBER 8

class Actor
{
private:
	class LevelInfo* LevelInfo;
	class AISystemBase* AISystem;
	Blackboard Blackboard;
	Actor* NearestEnemy;
	sf::Sprite RobotSprites[ROBOT_SPRITES_NUMBER];
	sf::Sprite LaserBeamSprite;
	sf::Sprite LaserBurstSprite;
	sf::Vector2f LastQuadTreePosition;
	sf::Vector2f Position;
	sf::Vector2f DesiredMovementDirection;
	sf::Vector2f ActualMovementDirection;
	sf::Vector2u Size;
	sf::Vector2u BeamTexSize;
	sf::Vector2f VectorTowardsEnemy;
	float ShotDist;
	sf::Vector2f MovementDirectionOffset;
	float MovementSpeed;
	float DirectionChangeSpeed;
	float MaxHP;
	float HP;
	float Damage;
	ETeam Team;
	sf::Vector2f MovementDirectionInterpStart;
	float MovementDirectionInterpAlpha;
	bool bInterpolateMovementDirection;
	bool bShouldDrawLaser;
	sf::Time ShotInterval;
	sf::Time QuadTreeUpdateInterval;
	sf::Time MovementDirectionUpdateInterval;
	sf::Time ShotTimeCounter;
	sf::Time QuadTreeUpdateCounter;
	sf::Time MovementDirectionUpdateTimeCounter;

	std::atomic<float> DrawData_PositionX;
	std::atomic<float> DrawData_PositionY;
	std::atomic<float> DrawData_LaserBeamDirectionX;
	std::atomic<float> DrawData_LaserBeamDirectionY;
	std::atomic<float> DrawData_HP;
	mutable float DrawData_AngleToEnemy;
	std::atomic<bool> DrawData_bShouldDrawLaser;

public:
	Actor(class LevelInfo* argLevelInfo, class TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition);
	~Actor();

	void SyncDrawData();
	void DrawRobot(sf::RenderWindow* Window) const;
	const sf::Sprite& GetRobotSprite() const;
	void DrawLaserBurst(sf::RenderWindow* Window) const;
	void DrawLaserBeam(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime);
	void UpdateAISystem();
	void ProcessMovement(const float DeltaTime);
	void UpdatePositionInQuadTree();
	void RetreatToHealZone();
	void CalculateVectorTowardsEnemy();
	void StopMovement();
	void GoTowardsNearestEnemy();
	void TryToShoot();
	void TryToShootToEnemyCapturePoint();
	void TakeDamage(float DamageAmount);
	const sf::Vector2f& GetPosition() const;
	ETeam GetTeam() const;
	void SetNearestEnemy(Actor* NewNearestEnemy);
	Actor* GetNearestEnemy() const;
	void UpdateLastQuadTreePosition(const sf::Vector2f& NewPositionInQuadTree);
	const sf::Vector2f& GetLastQuadTreePosition() const;
	void Heal(float HPToHeal);
	void SetBEnemyCapturePointAtLowHP(bool argbEnemyCapturePointAtLowHP);

private:
	void SetDesiredMovementDirection(const sf::Vector2f& NewDesiredMovementDireciton);
	void GenerateRandomMovementDirection(EDirection DirectionToAvoid = EDirection::NONE);
};
