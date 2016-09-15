// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

#include "HealZone.h"
#include "QuadTree.h"

//#define DRAW_DEBUG_GRID

#define ACTORS_PER_TEAM_NUMBER 3000
#define ACTORS_NUMBER (2 * ACTORS_PER_TEAM_NUMBER)

class LevelInfo
{
public:
	const sf::FloatRect Boundaries;
	const sf::Vector2f RightBottomEdge;

private:
	QuadTree QuadTree_TeamA;
	QuadTree QuadTree_TeamB;
	sf::Sprite BackgroundSprite;
#if defined DRAW_DEBUG_GRID
	sf::Sprite DebugGridSprite;
#endif
	class Actor* Actors[ACTORS_NUMBER];
	class Actor* ActorsTeamA[ACTORS_PER_TEAM_NUMBER];
	class Actor* ActorsTeamB[ACTORS_PER_TEAM_NUMBER];
	HealZone HealZoneA;
	HealZone HealZoneB;

public:
	LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries);
	~LevelInfo();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime, const sf::Time MainTimeCounter);
	void FindNearestEnemyForActor(class Actor* RequestingActor);
	void QuickFindNearEnemyForActor(class Actor* RequestingActor);
	void DestroyActor(class Actor* ActorToDestroy);
	void UpdatePositionInQuadTree(Actor* ActorToUpdate);
	sf::Vector2f GetHealZonePosition(ETeam Team) const;

private:
	static sf::Vector2f GetRandomPointInRect(const sf::FloatRect& Rect);
};
