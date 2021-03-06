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
	class CapturePoint* NearestEnemyCapturePoint;
	sf::Sprite RobotSprites[ROBOT_SPRITES_NUMBER];
	sf::Sprite LaserBeamSprite;
	sf::Sprite LaserBurstSprite;
	sf::Vector2f LastQuadTreePosition;
	sf::Vector2f Position;
	sf::Vector2f DesiredMovementDirection;
	sf::Vector2f ActualMovementDirection;
	sf::Vector2u Size;
	sf::Vector2u BeamTexSize;
	float ShotDist;
	const float RetreatingMovementSpeed;
	const float StandardMovementSpeed;
	sf::Vector2f MovementDirectionOffset;
	float CurrentMovementSpeed;
	float DirectionChangeSpeed;
	float MaxHP;
	float HP;
	float LowHPThreshold;
	float Damage;
	ETeam Team;
	sf::Vector2f MovementDirectionInterpStart;
	float MovementDirectionInterpAlpha;
	bool bInterpolateMovementDirection;
	bool bShouldDrawLaser;
	bool bRetreating;
	sf::Time ShotInterval;
	sf::Time QuadTreeUpdateInterval;
	sf::Time MovementDirectionUpdateInterval;
	sf::Time BBUpdateInterval;
	sf::Time ShotTimeCounter;
	sf::Time QuadTreeUpdateCounter;
	sf::Time MovementDirectionUpdateTimeCounter;
	sf::Time BBUpdateTimeCounter;

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
	sf::Vector2f CalculateVectorTowardsNearestEnemy() const;
	void StopMovement();
	void GoTowardsNearestEnemy();
	void GoTowardsEnemyCapturePoint();
	void GoTowardsAlliedCapturePoint();
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
	void NotifyNewEndangeredAlliedCapturePoint(CapturePoint* NewEndangeredAlliedCapturePoint);
	CapturePoint* GetNearestEnemyCapturePoint() const;
	void SetReatreating(bool bNewRetreating);
	bool IsRetrating() const;

private:
	void SetDesiredMovementDirection(const sf::Vector2f& NewDesiredMovementDireciton);
};
