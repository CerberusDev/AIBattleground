// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>
#include <mutex>

#include "HealZone.h"
#include "QuadTree.h"
#include "BTBase.h"

//#define DRAW_DEBUG_GRID

#define ACTORS_NUMBER 10000
#define CAPTURE_POINTS_PER_TEAM_NUMER 4
#define ACTOR_SPAWNERS_NUMER 6

class LevelInfo
{
public:
	const sf::FloatRect Boundaries;
	const sf::Vector2f RightBottomEdge;
	//sf::Time T1;
	//sf::Time T2;
	//sf::Time T3;
	sf::Time T4;

private:
	mutable std::mutex DrawDataMutex;
	QuadTree QuadTree_TeamA;
	QuadTree QuadTree_TeamB;
	sf::Sprite BackgroundSprite;
#if defined DRAW_DEBUG_GRID
	sf::Sprite DebugGridSprite;
#endif
	class Actor* Actors[ACTORS_NUMBER];
	Actor* Draw_Actors[ACTORS_NUMBER];
	std::vector<Actor*> ActorsToDelete;
	HealZone HealZoneA;
	HealZone HealZoneB;
	class ActorSpawner* ActorSpawners[ACTOR_SPAWNERS_NUMER];
	class CapturePoint* CapturePointsA[CAPTURE_POINTS_PER_TEAM_NUMER];
	CapturePoint* CapturePointsB[CAPTURE_POINTS_PER_TEAM_NUMER];
	CapturePoint* MostEndangeredCapturePointA;
	CapturePoint* MostEndangeredCapturePointB;
	BTBase BTData;

public:
	LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries);
	~LevelInfo();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime, const sf::Time FixedDeltaTime);
	void SyncData();
	void FindNearestEnemyForActor(class Actor* RequestingActor);
	void UpdateMostEndangeredCapturePoint(ETeam argTeam);
	void QuickFindNearEnemyForActor(class Actor* RequestingActor);
	void DestroyActor(class Actor* ActorToDestroy);
	void RegiseterInQuadTree(Actor* ActorToUpdate);
	void UnregiseterFromQuadTree(Actor* ActorToRegister);
	void UpdatePositionInQuadTree(Actor* ActorToUnregister);
	sf::Vector2f GetHealZonePosition(ETeam Team) const;
	class Actor** GetActorsArray();
	int GetActorsNumber() const;
	BTBase* GetBTData();
	CapturePoint* GetNearestEnemyCapturePoint(Actor* TargetActor);
	CapturePoint* GetMostEndangeredCapturePoint(ETeam argTeam) const;

private:
	static sf::Vector2f GetRandomPointInRect(const sf::FloatRect& Rect);
};
