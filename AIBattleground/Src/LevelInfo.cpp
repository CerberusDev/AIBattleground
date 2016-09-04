// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "LevelInfo.h"
#include "Actor.h"
#include "TextureManager.h"

LevelInfo::LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries) :
Boundaries(LevelBoundaries), RightBottomEdge(LevelBoundaries.left + LevelBoundaries.width, LevelBoundaries.top + LevelBoundaries.height)
{
	const float InitialRectSize = 0.15f;

	ETeam InitialTeam = ETeam::TEAM_A;
	std::string TexName("RobotA");
	Actor** ArrayToPopulate = ActorsTeamA;
	sf::FloatRect RectToSpawnIn(Boundaries.left, Boundaries.top, Boundaries.width * InitialRectSize, Boundaries.height);

	for (int i = 0; i < ACTORS_AMOUNT; ++i)
	{
		if (i == ACTORS_PER_TEAM_AMOUNT)
		{
			InitialTeam = ETeam::TEAM_B;
			TexName = "RobotB";
			ArrayToPopulate = ActorsTeamB;
			RectToSpawnIn = sf::FloatRect(Boundaries.left + Boundaries.width * (1.0f - InitialRectSize), Boundaries.top, Boundaries.width * InitialRectSize, Boundaries.height);
		}

		Actors[i] = new Actor(this, TexManager, TexName, InitialTeam, GetRandomPointInRect(RectToSpawnIn));
		ArrayToPopulate[i % ACTORS_PER_TEAM_AMOUNT] = Actors[i];
	}

	TexManager->InitTexture(&BackgroundSprite, "Background256", true);
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (int)Boundaries.width, (int)Boundaries.height));
	BackgroundSprite.setPosition(sf::Vector2f(Boundaries.left, Boundaries.top));

#if defined DRAW_DEBUG_GRID
	TexManager->InitTexture(&DebugGridSprite, "DebugGrid1000x800");
#endif
}

LevelInfo::~LevelInfo()
{
	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		if (Actors[i])
			delete Actors[i];
}

sf::Vector2f LevelInfo::GetRandomPointInRect(const sf::FloatRect& Rect)
{
	return sf::Vector2f(Rect.left + GetRandomFloat(Rect.width), Rect.top + GetRandomFloat(Rect.height));
}

void LevelInfo::Draw(sf::RenderWindow* Window) const
{
	Window->draw(BackgroundSprite);

#if defined DRAW_DEBUG_GRID
	Window->draw(DebugGridSprite);
#endif

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->DrawBeam(Window);

	const int Divider = 10; // ACTORS_AMOUNT has to be divisible by this value!

	for (int i = 0; i < Divider; ++i)
	{
		int ii;

		if (i % 2 == 0)
			ii = i / 2;
		else
			ii = Divider - 1 - i / 2;

		// for Divider == 6 "ii" will looks like this:  0      5       1       4       2       3
		// so for 600 actors "j" will looks like this: 0-99 500-599 100-199 400-499 200-299 300-399

		for (int j = ii * ACTORS_AMOUNT / Divider; j < (ii + 1) * ACTORS_AMOUNT / Divider; ++j)
		{
			if (Actors[j])
			{
				Actors[j]->DrawRobot(Window);
			}
		}
	}
}

void LevelInfo::Update(const float DeltaTime, const sf::Time MainTimeCounter)
{
	static int LastIndex = 0;

	int NewIndex = (int)(MainTimeCounter.asSeconds() * ACTORS_AMOUNT);

	if (NewIndex < LastIndex)
		NewIndex = ACTORS_AMOUNT;

	for (int i = LastIndex; i < NewIndex; ++i)
		if (Actors[i])
			FindNearestEnemyForActor(Actors[i]);

	LastIndex = NewIndex != ACTORS_AMOUNT ? NewIndex: 0;

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->Update(DeltaTime);
}

void LevelInfo::FindNearestEnemyForActor(class Actor* RequestingActor)
{
	Actor* NearestOtherActor = nullptr;
	float MinSquaredDist = FLT_MAX;

	Actor** EnemyActors = RequestingActor->GetTeam() == ETeam::TEAM_A ? ActorsTeamB : ActorsTeamA;

	for (int j = 0; j < ACTORS_PER_TEAM_AMOUNT; ++j)
	{
		Actor* CurrEnemyActor = EnemyActors[j];

		if (CurrEnemyActor)
		{
			const float SquaredDist = GetSquaredDist(RequestingActor->GetPosition(), CurrEnemyActor->GetPosition());

			if (SquaredDist < MinSquaredDist)
			{
				MinSquaredDist = SquaredDist;
				NearestOtherActor = CurrEnemyActor;
			}
		}
	}

	RequestingActor->SetNearestEnemy(NearestOtherActor);
}

void LevelInfo::DestroyActor(class Actor* ActorToDestroy)
{
	for (int i = 0; i < ACTORS_AMOUNT; ++i)
	{
		if (Actors[i] == ActorToDestroy)
		{
			Actors[i] = nullptr;
			break;
		}
	}

	Actor** TeamActors = ActorToDestroy->GetTeam() == ETeam::TEAM_A ? ActorsTeamA : ActorsTeamB;

	for (int i = 0; i < ACTORS_PER_TEAM_AMOUNT; ++i)
	{
		if (TeamActors[i] == ActorToDestroy)
		{
			TeamActors[i] = nullptr;
			break;
		}
	}

	Actor** EnemyActors = ActorToDestroy->GetTeam() == ETeam::TEAM_A ? ActorsTeamB : ActorsTeamA;

	for (int i = 0; i < ACTORS_PER_TEAM_AMOUNT; ++i)
		if (EnemyActors[i] && EnemyActors[i]->GetNearestEnemy() == ActorToDestroy)
			FindNearestEnemyForActor(EnemyActors[i]);

	delete ActorToDestroy;
}
