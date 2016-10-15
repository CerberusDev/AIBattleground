// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Globals.h"

class ActorSpawner
{
private:
	class LevelInfo* LevelInfo;
	const sf::Vector2f Position;
	const ETeam Team;
	const float Radius;
	const int ActorsPerSecond;
	class Actor** ActorsArray;
	const int ActorsNumber;
	class TextureManager* TexManager;
	const std::string ActorTexName;
	const int TeamOffset;
	sf::Time SpawnInterval;
	sf::Time SpawnTimeCounter;

public:
	ActorSpawner(class LevelInfo* argLevelInfo, class TextureManager* argTexManager, sf::Vector2f argPosition, ETeam argTeam, int argActorsPerSecond);
	~ActorSpawner();

	void Update(const float DeltaTime);
};
