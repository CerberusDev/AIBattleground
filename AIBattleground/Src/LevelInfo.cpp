// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "LevelInfo.h"
#include "Actor.h"
#include "TextureManager.h"
#include "CapturePoint.h"
#include "ActorSpawner.h"

LevelInfo::LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries) :
Boundaries(LevelBoundaries), RightBottomEdge(LevelBoundaries.left + LevelBoundaries.width, LevelBoundaries.top + LevelBoundaries.height),
QuadTree_TeamA(sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.5f, LevelBoundaries.top + LevelBoundaries.height * 0.5f)),
QuadTree_TeamB(sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.5f, LevelBoundaries.top + LevelBoundaries.height * 0.5f)),
HealZoneA(TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.125f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_A, Actors, ACTORS_NUMBER),
HealZoneB(TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.875f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_B, Actors, ACTORS_NUMBER),
MostEndangeredCapturePointA(nullptr), MostEndangeredCapturePointB(nullptr)
{
	for (int i = 0; i < ACTORS_NUMBER; ++i)
		Actors[i] = nullptr;

	TexManager->InitTexture(&BackgroundSprite, "Background256", true);
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (int)Boundaries.width, (int)Boundaries.height));
	BackgroundSprite.setPosition(sf::Vector2f(Boundaries.left, Boundaries.top));

	ActorSpawners[0] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.025f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_A, 350);
	ActorSpawners[1] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.025f, LevelBoundaries.top + LevelBoundaries.height * 0.3f), ETeam::TEAM_A, 350);
	ActorSpawners[2] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.025f, LevelBoundaries.top + LevelBoundaries.height * 0.7f), ETeam::TEAM_A, 200);
	ActorSpawners[3] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.975f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_B, 300);
	ActorSpawners[4] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.975f, LevelBoundaries.top + LevelBoundaries.height * 0.3f), ETeam::TEAM_B, 300);
	ActorSpawners[5] = new ActorSpawner(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.975f, LevelBoundaries.top + LevelBoundaries.height * 0.7f), ETeam::TEAM_B, 300);

	CapturePointsA[0] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.275f, LevelBoundaries.top + LevelBoundaries.height * 0.125f), ETeam::TEAM_A);
	CapturePointsA[1] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.25f, LevelBoundaries.top + LevelBoundaries.height * 0.375f), ETeam::TEAM_A);
	CapturePointsA[2] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.25f, LevelBoundaries.top + LevelBoundaries.height * 0.625f), ETeam::TEAM_A);
	CapturePointsA[3] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.275f, LevelBoundaries.top + LevelBoundaries.height * 0.875f), ETeam::TEAM_A);

	CapturePointsB[0] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.725f, LevelBoundaries.top + LevelBoundaries.height * 0.125f), ETeam::TEAM_B);
	CapturePointsB[1] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.75f, LevelBoundaries.top + LevelBoundaries.height * 0.375f), ETeam::TEAM_B);
	CapturePointsB[2] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.75f, LevelBoundaries.top + LevelBoundaries.height * 0.625f), ETeam::TEAM_B);
	CapturePointsB[3] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.725f, LevelBoundaries.top + LevelBoundaries.height * 0.875f), ETeam::TEAM_B);


#if defined DRAW_DEBUG_GRID
	TexManager->InitTexture(&DebugGridSprite, "DebugGrid1000x800");
#endif
}

LevelInfo::~LevelInfo()
{
	for (int i = 0; i < ACTORS_NUMBER; ++i)
		if (Actors[i])
			delete Actors[i];

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		delete CapturePointsA[i];
		delete CapturePointsB[i];
	}

	for (int i = 0; i < ACTOR_SPAWNERS_NUMER; ++i)
	{
		delete ActorSpawners[i];
	}
}

sf::Vector2f LevelInfo::GetRandomPointInRect(const sf::FloatRect& Rect)
{
	return sf::Vector2f(Rect.left + GetRandomFloat(Rect.width), Rect.top + GetRandomFloat(Rect.height));
}

void LevelInfo::Draw(sf::RenderWindow* Window) const
{
	DrawDataMutex.lock();

	Window->draw(BackgroundSprite);

#if defined DRAW_DEBUG_GRID
	Window->draw(DebugGridSprite);
#endif

	HealZoneA.Draw(Window);
	HealZoneB.Draw(Window);

	for (Actor* CurrActor : Draw_Actors)
		if (CurrActor)
			CurrActor->DrawLaserBeam(Window);

	for (Actor* CurrActor : Draw_Actors)
		if (CurrActor)
			CurrActor->DrawRobot(Window);

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		CapturePointsA[i]->Draw(Window);
		CapturePointsB[i]->Draw(Window);
	}

	DrawDataMutex.unlock();
}

void LevelInfo::Update(const float DeltaTime, const sf::Time FixedDeltaTime)
{
	static int LastIndex = 0;

	int NewIndex = LastIndex + std::max((int)(FixedDeltaTime.asSeconds() * 1.5f * ACTORS_NUMBER), 1);

	if (NewIndex > ACTORS_NUMBER)
		NewIndex = ACTORS_NUMBER;

	for (int i = LastIndex; i < NewIndex; ++i)
		if (Actors[i])
			FindNearestEnemyForActor(Actors[i]);

	LastIndex = NewIndex != ACTORS_NUMBER ? NewIndex: 0;

	//T1 += C.restart();

	for (ActorSpawner* CurrSpawner : ActorSpawners)
		CurrSpawner->Update(DeltaTime);

	HealZoneA.Update(DeltaTime);
	HealZoneB.Update(DeltaTime);

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		CapturePointsA[i]->Update(DeltaTime);
		CapturePointsB[i]->Update(DeltaTime);
	}

	UpdateMostEndangeredCapturePoint(ETeam::TEAM_A);
	UpdateMostEndangeredCapturePoint(ETeam::TEAM_B);

	//T2 += C.restart();

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->Update(DeltaTime);

	//T3 += C.restart();
	sf::Clock C;

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->UpdateAISystem();

	T4 += C.restart();
}

void LevelInfo::SyncData()
{
	DrawDataMutex.lock();

	while (!ActorsToDelete.empty())
	{
		Actor* ActorToDelete = ActorsToDelete.back();
		ActorsToDelete.pop_back();
		delete ActorToDelete;
	}

	int DrawIdx = 0;

	for (int i = 0; i < ACTORS_NUMBER; ++i)
	{
		if (Actors[i])
		{
			if (Actors[i]->IsRetrating())
				Draw_Actors[DrawIdx++] = Actors[i];

			Actors[i]->SyncDrawData();
		}
	}

	for (int i = 0; i < ACTORS_NUMBER; ++i)
		if (Actors[i] && !Actors[i]->IsRetrating())
			Draw_Actors[DrawIdx++] = Actors[i];

	for (int i = DrawIdx; i < ACTORS_NUMBER; ++i)
		Draw_Actors[i] = nullptr;

	DrawDataMutex.unlock();
}

void LevelInfo::UpdateMostEndangeredCapturePoint(ETeam argTeam)
{
	CapturePoint** MostEndangeredCapturePoint = argTeam == ETeam::TEAM_A ? &MostEndangeredCapturePointA : &MostEndangeredCapturePointB;
	CapturePoint** CapturePoints = argTeam == ETeam::TEAM_A ? CapturePointsA : CapturePointsB;
	CapturePoint* NewMostEndangeredCapturePoint = nullptr;
	float MinHP = FLT_MAX;

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		if (CapturePoints[i]->HasLowHP() && CapturePoints[i]->GetHP() < MinHP)
		{
			NewMostEndangeredCapturePoint = CapturePoints[i];
			MinHP = CapturePoints[i]->GetHP();

			if (NewMostEndangeredCapturePoint->HasVeryLowHP())
				break;
		}
	}

	if (NewMostEndangeredCapturePoint != *MostEndangeredCapturePoint)
	{
		*MostEndangeredCapturePoint = NewMostEndangeredCapturePoint;

		int TeamOffset = argTeam == ETeam::TEAM_A ? 0 : ACTORS_NUMBER / 2;

		for (int i = TeamOffset; i < ACTORS_NUMBER / 2 + TeamOffset; ++i)
			if (Actors[i])
				Actors[i]->NotifyNewEndangeredAlliedCapturePoint(*MostEndangeredCapturePoint);
	}
}

void LevelInfo::FindNearestEnemyForActor(class Actor* RequestingActor)
{
	QuadTree* EnemyQuadTree = RequestingActor->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamB : &QuadTree_TeamA;
	Actor* NearestNeighbor = EnemyQuadTree->FindNearestNeighborTo(RequestingActor->GetPosition());
	RequestingActor->SetNearestEnemy(NearestNeighbor);
}

void LevelInfo::QuickFindNearEnemyForActor(class Actor* RequestingActor)
{
	QuadTree* EnemyQuadTree = RequestingActor->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamB : &QuadTree_TeamA;
	RequestingActor->SetNearestEnemy(EnemyQuadTree->QuickFindNearNeighborTo(RequestingActor));
}

void LevelInfo::DestroyActor(class Actor* ActorToDestroy)
{
	QuadTree* QuadTreeToRemoveFrom = ActorToDestroy->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamA : &QuadTree_TeamB;
	QuadTreeToRemoveFrom->RemoveActor(ActorToDestroy);

	for (int i = 0; i < ACTORS_NUMBER; ++i)
	{
		if (Actors[i] == ActorToDestroy)
		{
			Actors[i] = nullptr;
			break;
		}
	}

	int TeamOffset = ActorToDestroy->GetTeam() == ETeam::TEAM_A ? ACTORS_NUMBER / 2 : 0;

	for (int i = TeamOffset; i < ACTORS_NUMBER / 2 + TeamOffset; ++i)
		if (Actors[i] && Actors[i]->GetNearestEnemy() == ActorToDestroy)
			QuickFindNearEnemyForActor(Actors[i]);

	ActorsToDelete.push_back(ActorToDestroy);
}

void LevelInfo::RegiseterInQuadTree(Actor* ActorToRegister)
{
	QuadTree* QuadTreeToUpdate = ActorToRegister->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamA : &QuadTree_TeamB;
	QuadTreeToUpdate->AddActor(ActorToRegister);
}

void LevelInfo::UnregiseterFromQuadTree(Actor* ActorToUnregister)
{
	QuadTree* QuadTreeToUpdate = ActorToUnregister->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamA : &QuadTree_TeamB;
	QuadTreeToUpdate->RemoveActor(ActorToUnregister);
}

void LevelInfo::UpdatePositionInQuadTree(Actor* ActorToUpdate)
{
	QuadTree* QuadTreeToUpdate = ActorToUpdate->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamA : &QuadTree_TeamB;
	QuadTreeToUpdate->RemoveActor(ActorToUpdate);
	QuadTreeToUpdate->AddActor(ActorToUpdate);
}

sf::Vector2f LevelInfo::GetHealZonePosition(ETeam Team) const
{
	return Team == ETeam::TEAM_A ? HealZoneA.GetPosition() : HealZoneB.GetPosition();
}

class Actor** LevelInfo::GetActorsArray()
{
	return Actors;
}

int LevelInfo::GetActorsNumber() const
{
	return ACTORS_NUMBER;
}

BTBase* LevelInfo::GetBTData()
{
	return &BTData;
}

CapturePoint* LevelInfo::GetNearestEnemyCapturePoint(Actor* TargetActor)
{
	CapturePoint** AvailableCapturePoints = TargetActor->GetTeam() == ETeam::TEAM_A ? CapturePointsB : CapturePointsA;

	CapturePoint* NearestCapturePoint = AvailableCapturePoints[0];
	float MinSquaredDist = GetSquaredDist(TargetActor->GetPosition(), NearestCapturePoint->GetPosition());

	for (int i = 1; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		float SquaredDist = GetSquaredDist(TargetActor->GetPosition(), AvailableCapturePoints[i]->GetPosition());

		if (SquaredDist < MinSquaredDist)
		{
			MinSquaredDist = SquaredDist;
			NearestCapturePoint = AvailableCapturePoints[i];
		}
	}

	return NearestCapturePoint;
}

CapturePoint* LevelInfo::GetMostEndangeredCapturePoint(ETeam argTeam) const
{
	return argTeam == ETeam::TEAM_A ? MostEndangeredCapturePointA : MostEndangeredCapturePointB;
}
