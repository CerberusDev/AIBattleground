// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "LevelInfo.h"
#include "Actor.h"
#include "TextureManager.h"
#include "CapturePoint.h"

LevelInfo::LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries) :
Boundaries(LevelBoundaries), RightBottomEdge(LevelBoundaries.left + LevelBoundaries.width, LevelBoundaries.top + LevelBoundaries.height),
QuadTree_TeamA(sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.5f, LevelBoundaries.top + LevelBoundaries.height * 0.5f)),
QuadTree_TeamB(sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.5f, LevelBoundaries.top + LevelBoundaries.height * 0.5f)),
HealZoneA(TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.125f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_A, Actors, ACTORS_NUMBER),
HealZoneB(TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.875f, LevelBoundaries.top + LevelBoundaries.height * 0.5f), ETeam::TEAM_B, Actors, ACTORS_NUMBER),
SpawnerA1(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.05f, LevelBoundaries.top + LevelBoundaries.height * 0.85f), ETeam::TEAM_A, 150),
SpawnerA2(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.05f, LevelBoundaries.top + LevelBoundaries.height * 0.15f), ETeam::TEAM_A, 30),
SpawnerB1(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.95f, LevelBoundaries.top + LevelBoundaries.height * 0.15f), ETeam::TEAM_B, 150),
SpawnerB2(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.95f, LevelBoundaries.top + LevelBoundaries.height * 0.85f), ETeam::TEAM_B, 30)
{
	for (int i = 0; i < ACTORS_NUMBER; ++i)
		Actors[i] = nullptr;

	TexManager->InitTexture(&BackgroundSprite, "Background256", true);
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (int)Boundaries.width, (int)Boundaries.height));
	BackgroundSprite.setPosition(sf::Vector2f(Boundaries.left, Boundaries.top));

	CapturePointsA[0] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.375f, LevelBoundaries.top + LevelBoundaries.height * 0.3f), ETeam::TEAM_A);
	CapturePointsA[1] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.375f, LevelBoundaries.top + LevelBoundaries.height * 0.7f), ETeam::TEAM_A);

	CapturePointsB[0] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.625f, LevelBoundaries.top + LevelBoundaries.height * 0.3f), ETeam::TEAM_B);
	CapturePointsB[1] = new CapturePoint(this, TexManager, sf::Vector2f(LevelBoundaries.left + LevelBoundaries.width * 0.625f, LevelBoundaries.top + LevelBoundaries.height * 0.7f), ETeam::TEAM_B);


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

	HealZoneA.Draw(Window);
	HealZoneB.Draw(Window);

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->DrawLaserBeam(Window);

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->DrawRobot(Window);

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		CapturePointsA[i]->Draw(Window);
		CapturePointsB[i]->Draw(Window);
	}
}

void LevelInfo::Update(const float DeltaTime, const sf::Time FixedDeltaTime)
{
	sf::Clock C;

	static int LastIndex = 0;

	int NewIndex = LastIndex + std::max((int)(FixedDeltaTime.asSeconds() * 1.5f * ACTORS_NUMBER), 1);

	if (NewIndex > ACTORS_NUMBER)
		NewIndex = ACTORS_NUMBER;

	for (int i = LastIndex; i < NewIndex; ++i)
		if (Actors[i])
			FindNearestEnemyForActor(Actors[i]);

	LastIndex = NewIndex != ACTORS_NUMBER ? NewIndex: 0;

	T1 += C.restart();

	SpawnerA1.Update(DeltaTime);
	SpawnerA2.Update(DeltaTime);
	SpawnerB1.Update(DeltaTime);
	SpawnerB2.Update(DeltaTime);
	HealZoneA.Update(DeltaTime);
	HealZoneB.Update(DeltaTime);

	for (int i = 0; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		CapturePointsA[i]->Update(DeltaTime);
		CapturePointsB[i]->Update(DeltaTime);
	}

	T2 += C.restart();

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->Update(DeltaTime);

	T3 += C.restart();

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->UpdateAISystem();

	T4 += C.restart();

	for (Actor* CurrActor : Actors)
		if (CurrActor)
			CurrActor->SyncDrawData();

	T5 += C.restart();
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

	delete ActorToDestroy;
}

void LevelInfo::InitPositionInQuadTree(Actor* ActorToUpdate)
{
	QuadTree* QuadTreeToUpdate = ActorToUpdate->GetTeam() == ETeam::TEAM_A ? &QuadTree_TeamA : &QuadTree_TeamB;
	QuadTreeToUpdate->AddActor(ActorToUpdate);
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
	return GetNearestCapturePoint(TargetActor->GetPosition(), TargetActor->GetTeam() == ETeam::TEAM_A ? ETeam::TEAM_B : ETeam::TEAM_A);
}

CapturePoint* LevelInfo::GetNearestAlliedCapturePoint(Actor* TargetActor)
{
	return GetNearestCapturePoint(TargetActor->GetPosition(), TargetActor->GetTeam());
}

CapturePoint* LevelInfo::GetNearestCapturePoint(const sf::Vector2f& TargetPosition, ETeam argTeam)
{
	CapturePoint** AvailableCapturePoints = argTeam == ETeam::TEAM_A ? CapturePointsA : CapturePointsB;

	CapturePoint* NearestCapturePoint = AvailableCapturePoints[0];
	float MinSquaredDist = GetSquaredDist(TargetPosition, NearestCapturePoint->GetPosition());

	for (int i = 1; i < CAPTURE_POINTS_PER_TEAM_NUMER; ++i)
	{
		float SquaredDist = GetSquaredDist(TargetPosition, AvailableCapturePoints[i]->GetPosition());

		if (SquaredDist < MinSquaredDist)
		{
			MinSquaredDist = SquaredDist;
			NearestCapturePoint = AvailableCapturePoints[i];
		}
	}

	return NearestCapturePoint;
}
