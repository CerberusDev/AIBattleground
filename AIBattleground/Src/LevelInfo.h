// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

#include "HealZone.h"
#include "ActorSpawner.h"
#include "QuadTree.h"
#include "BTBase.h"

//#define DRAW_DEBUG_GRID

#define ACTORS_NUMBER 6000
#define CAPTURE_POINTS_PER_TEAM_NUMER 2

class LevelInfo
{
public:
	const sf::FloatRect Boundaries;
	const sf::Vector2f RightBottomEdge;
	sf::Time T1;
	sf::Time T2;
	sf::Time T3;
	sf::Time T4;
	sf::Time T5;

private:
	QuadTree QuadTree_TeamA;
	QuadTree QuadTree_TeamB;
	sf::Sprite BackgroundSprite;
#if defined DRAW_DEBUG_GRID
	sf::Sprite DebugGridSprite;
#endif
	class Actor* Actors[ACTORS_NUMBER];
	HealZone HealZoneA;
	HealZone HealZoneB;
	ActorSpawner SpawnerA1;
	ActorSpawner SpawnerA2;
	ActorSpawner SpawnerB1;
	ActorSpawner SpawnerB2;
	class CapturePoint* CapturePointsA[CAPTURE_POINTS_PER_TEAM_NUMER];
	CapturePoint* CapturePointsB[CAPTURE_POINTS_PER_TEAM_NUMER];
	BTBase BTData;

public:
	LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries);
	~LevelInfo();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime, const sf::Time FixedDeltaTime);
	void FindNearestEnemyForActor(class Actor* RequestingActor);
	void QuickFindNearEnemyForActor(class Actor* RequestingActor);
	void DestroyActor(class Actor* ActorToDestroy);
	void InitPositionInQuadTree(Actor* ActorToUpdate);
	void UpdatePositionInQuadTree(Actor* ActorToUpdate);
	sf::Vector2f GetHealZonePosition(ETeam Team) const;
	class Actor** GetActorsArray();
	int GetActorsNumber() const;
	BTBase* GetBTData();
	CapturePoint* GetNearestEnemyCapturePoint(Actor* TargetActor);
	CapturePoint* GetNearestAlliedCapturePoint(Actor* TargetActor);

private:
	static sf::Vector2f GetRandomPointInRect(const sf::FloatRect& Rect);
	CapturePoint* GetNearestCapturePoint(const sf::Vector2f& TargetPosition, ETeam argTeam);
};
