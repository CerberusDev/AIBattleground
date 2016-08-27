// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

#define ACTORS_PER_TEAM_AMOUNT 500
#define ACTORS_AMOUNT (2 * ACTORS_PER_TEAM_AMOUNT)

class LevelInfo
{
public:
	const sf::FloatRect Boundaries;
	const sf::Vector2f RightBottomEdge;

private:
	sf::Sprite BackgroundSprite;
	class Actor* Actors[ACTORS_AMOUNT];
	class Actor* ActorsTeamA[ACTORS_PER_TEAM_AMOUNT];
	class Actor* ActorsTeamB[ACTORS_PER_TEAM_AMOUNT];

public:
	LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries);
	~LevelInfo();

	void Draw(sf::RenderWindow* Window) const;
	void Update(const float DeltaTime, const sf::Time MainTimeCounter);
};