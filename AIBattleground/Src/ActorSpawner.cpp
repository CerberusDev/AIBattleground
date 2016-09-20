// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "ActorSpawner.h"
#include "LevelInfo.h"

ActorSpawner::ActorSpawner(class LevelInfo* argLevelInfo, class TextureManager* argTexManager, sf::Vector2f argPosition, ETeam argTeam) :
LevelInfo(argLevelInfo), TexManager(argTexManager), Position(argPosition), Team(argTeam), Radius(20.0f), ActorsPerSecond(200),
ActorsArray(LevelInfo->GetActorsArray()), ActorsNumber(LevelInfo->GetActorsNumber()), 
ActorTexName(Team == ETeam::TEAM_A ? "RobotA" : "RobotB"), TeamOffset(Team == ETeam::TEAM_A ? 0 : ActorsNumber / 2),
SpawnInterval(sf::seconds(1.0f / ActorsPerSecond))
{
	
}

ActorSpawner::~ActorSpawner()
{

}

void ActorSpawner::Update(const float DeltaTime)
{
	SpawnTimeCounter += sf::seconds(DeltaTime);

	if (SpawnTimeCounter > SpawnInterval)
	{
		for (int i = TeamOffset; i < ActorsNumber / 2 + TeamOffset; ++i)
		{
			if (ActorsArray[i] == nullptr)
			{
				sf::Vector2f Dir(GetRandomFloat(1.0f) - 0.5f, GetRandomFloat(1.0f) - 0.5f);
				NormalizeVector2f(Dir);
				Actor* NewActor = new Actor(LevelInfo, TexManager, ActorTexName, Team, Position + Dir * GetRandomFloat(Radius));
				ActorsArray[i] = NewActor;
				SpawnTimeCounter -= SpawnInterval;

				if (SpawnTimeCounter < SpawnInterval)
					break;
			}
		}
	}
}
