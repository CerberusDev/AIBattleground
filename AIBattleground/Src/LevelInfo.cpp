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
	ETeam InitialTeam = ETeam::TEAM_A;
	std::string TexName("RobotA");
	Actor** ArrayToPopulate = ActorsTeamA;

	for (int i = 0; i < ACTORS_AMOUNT; ++i)
	{
		if (i == ACTORS_PER_TEAM_AMOUNT)
		{
			InitialTeam = ETeam::TEAM_B;
			TexName = "RobotB";
			ArrayToPopulate = ActorsTeamB;
		}

		const sf::Vector2f InitialPos((float)(std::rand() % (int)Boundaries.width), (float)(std::rand() % (int)Boundaries.height));
		Actors[i] = new Actor(this, TexManager, TexName, InitialTeam, InitialPos);
		ArrayToPopulate[i % ACTORS_PER_TEAM_AMOUNT] = Actors[i];
	}

	TexManager->InitTexture(&BackgroundSprite, "Background256", true);
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (int)Boundaries.width, (int)Boundaries.height));
}

LevelInfo::~LevelInfo()
{
	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		delete Actors[i];
}

void LevelInfo::Draw(sf::RenderWindow* Window) const
{
	Window->draw(BackgroundSprite);

	for (const Actor* CurrActor : Actors)
		CurrActor->Draw(Window);
}

void LevelInfo::Update(const float DeltaTime, const sf::Time MainTimeCounter)
{
	static int LastIndex = 0;

	int NewIndex = (int)(MainTimeCounter.asSeconds() * ACTORS_AMOUNT);

	if (NewIndex < LastIndex)
		NewIndex = ACTORS_AMOUNT;

	for (int i = LastIndex; i < NewIndex; ++i)
	{
		Actor* CurrActor = Actors[i];
		Actor* NearestOtherActor = nullptr;
		float MinSquaredDist = 9999999.0f;

		Actor** EnemyActors = CurrActor->GetTeam() == ETeam::TEAM_A ? ActorsTeamA : ActorsTeamB;

		for (int j = 0; j < ACTORS_PER_TEAM_AMOUNT; ++j)
		{
			Actor* CurrEnemyActor = EnemyActors[j];

			if (CurrEnemyActor != CurrActor)
			{
				const float SquaredDist = GetSquaredDist(CurrActor->GetPosition(), CurrEnemyActor->GetPosition());
				if (SquaredDist < MinSquaredDist)
				{
					MinSquaredDist = SquaredDist;
					NearestOtherActor = CurrEnemyActor;
				}
			}
		}

		if (NearestOtherActor)
			CurrActor->SetNearestEnemy(NearestOtherActor);
	}

	LastIndex = NewIndex != ACTORS_AMOUNT ? NewIndex: 0;

	for (Actor* CurrActor : Actors)
		CurrActor->Update(DeltaTime);
}