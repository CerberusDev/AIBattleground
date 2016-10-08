// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "Actor.h"
#include "TextureManager.h"
#include "LevelInfo.h"
#include "AISystemFSM.h"
#include "AISystemBT.h"

Actor::Actor(class LevelInfo* argLevelInfo, TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition) :
LevelInfo(argLevelInfo), AISystem(nullptr), Blackboard(this), NearestEnemy(nullptr), LastQuadTreePosition(InitialPosition), Position(InitialPosition), DesiredMovementDirection(0.0f, 0.0f),
ActualMovementDirection(0.0f, 0.0f), VectorTowardsEnemy(0.0f, 0.0f), ShotDist(75.0f * (1.0f - GetRandomFloat(0.4f))), MovementSpeed(100.0f), DirectionChangeSpeed(5.0f),
MaxHP(100.0f), HP(MaxHP), Damage(5.0f), Team(argTeam), MovementDirectionInterpStart(0.0f, 0.0f), bInterpolateMovementDirection(false),
MovementDirectionInterpAlpha(0.0f), bShouldDrawLaser(false), ShotTimeCounter(ShotInterval), QuadTreeUpdateCounter(sf::seconds(GetRandomFloat(QuadTreeUpdateInterval.asSeconds()))),
ShotInterval(sf::seconds(0.75f)), QuadTreeUpdateInterval(sf::seconds(0.2f)),
DrawData_PositionX(Position.x), DrawData_PositionY(Position.y), DrawData_VectorTowardsEnemyX(0.0f), DrawData_VectorTowardsEnemyY(0.0f),
DrawData_HP(MaxHP), DrawData_AngleToEnemy(0.0f), DrawData_bShouldDrawLaser(false)
{
	for (int i = 0; i < ROBOT_SPRITES_AMOUNT; ++i)
	{
		sf::Vector2u TexSize = TexManager->InitTexture(&RobotSprites[i], TexName + std::to_string(i + 1));

		if (i == 0)
			Size = TexSize;
		else if (TexSize != Size)
			std::cout << "Actor construction: sprite textures in different sizes! " << TexName << " " << i << std::endl;

		RobotSprites[i].setOrigin(Size.x / 2.0f, Size.y / 2.0f);
	}

	BeamTexSize = TexManager->InitTexture(&LaserBeamSprite, "LaserBeam");
	LaserBeamSprite.setOrigin(0.0f, BeamTexSize.y / 2.0f);

	sf::Vector2u BurstTexSize = TexManager->InitTexture(&LaserBurstSprite, "LaserBurst");
	LaserBurstSprite.setOrigin(BurstTexSize.x / 2.0f, BurstTexSize.y / 2.0f);

	MovementDirectionOffset.x = GetRandomFloat(ShotDist * 1.5f) - ShotDist * 0.75f;
	MovementDirectionOffset.y = GetRandomFloat(ShotDist * 1.5f) - ShotDist * 0.75f;

	//AISystem = new AISystemFSM(&Blackboard);
	AISystem = new AISystemBT(LevelInfo->GetBTData(), &Blackboard);
	//AISystem = new AISystemBT(new BTBase(), &Blackboard);

	Blackboard.SetMaxHP(MaxHP);
	Blackboard.SetHP(HP);

	LevelInfo->InitPositionInQuadTree(this);
	LevelInfo->FindNearestEnemyForActor(this);
}

Actor::~Actor()
{
	delete AISystem;
}

void Actor::SyncDrawData()
{
	DrawData_PositionX = Position.x;
	DrawData_PositionY = Position.y;
	DrawData_HP = HP;
	DrawData_bShouldDrawLaser = bShouldDrawLaser;
	DrawData_VectorTowardsEnemyX = VectorTowardsEnemy.x;
	DrawData_VectorTowardsEnemyY = VectorTowardsEnemy.y;
}

void Actor::DrawRobot(sf::RenderWindow* Window) const
{
	DrawLaserBurst(Window);

	sf::Transform CurrTransform;
	CurrTransform.translate(sf::Vector2f(DrawData_PositionX, DrawData_PositionY));

	Window->draw(GetRobotSprite(), CurrTransform);
}

const sf::Sprite& Actor::GetRobotSprite() const
{
	const float HPRatio = DrawData_HP / MaxHP;
	return RobotSprites[(int)((1.0f - HPRatio) * ROBOT_SPRITES_AMOUNT)];
}

void Actor::DrawLaserBeam(sf::RenderWindow* Window) const
{
	if (DrawData_bShouldDrawLaser)
	{
		sf::Vector2f Diff(DrawData_VectorTowardsEnemyX, DrawData_VectorTowardsEnemyY);
		NormalizeVector2f(Diff);
		DrawData_AngleToEnemy = acos(Diff.x);
		DrawData_AngleToEnemy *= (float)M_1_PI * 180.0f;

		if (Diff.y < 0.0f)
			DrawData_AngleToEnemy = 360.0f - DrawData_AngleToEnemy;

		sf::Transform CurrTransform;
		CurrTransform.translate(sf::Vector2f(DrawData_PositionX, DrawData_PositionY));
		CurrTransform.rotate(DrawData_AngleToEnemy);
		CurrTransform.scale(GetLength(sf::Vector2f(DrawData_VectorTowardsEnemyX, DrawData_VectorTowardsEnemyY)) / BeamTexSize.x, 1.0f);

		Window->draw(LaserBeamSprite, CurrTransform);
	}
}

void Actor::DrawLaserBurst(sf::RenderWindow* Window) const
{
	if (DrawData_bShouldDrawLaser)
	{
		sf::Transform CurrTransform;
		CurrTransform.translate(sf::Vector2f(DrawData_PositionX, DrawData_PositionY));
		CurrTransform.rotate(DrawData_AngleToEnemy);
		CurrTransform.translate(sf::Vector2f(Size.x / 2.0f + 1.0f, 0.0f));
		Window->draw(LaserBurstSprite, CurrTransform);
	}
}

void Actor::Update(const float DeltaTime)
{
	ShotTimeCounter += sf::seconds(DeltaTime);
	QuadTreeUpdateCounter += sf::seconds(DeltaTime);
	bShouldDrawLaser = false;
	
	ProcessMovement(DeltaTime);
	UpdatePositionInQuadTree();

	if (HP != MaxHP)
	{
		const bool bReached = GetSquaredDist(LevelInfo->GetHealZonePosition(Team) + MovementDirectionOffset, GetPosition()) < 100.0f;
		Blackboard.SetBHealthZoneDestReached(bReached);	
	}

	if (NearestEnemy)
	{
		VectorTowardsEnemy = NearestEnemy->GetPosition() - GetPosition();
		Blackboard.SetBEnemyInRange(GetLength(VectorTowardsEnemy) <= ShotDist);
	}
}

void Actor::UpdateAISystem()
{
	AISystem->Update();
}

void Actor::ProcessMovement(const float DeltaTime)
{
	if (bInterpolateMovementDirection)
	{
		if (MovementDirectionInterpAlpha >= 1.0f)
		{
			ActualMovementDirection = DesiredMovementDirection;
			bInterpolateMovementDirection = false;
		}
		else
		{
			MovementDirectionInterpAlpha += DeltaTime * DirectionChangeSpeed;
			ActualMovementDirection = (1.0f - MovementDirectionInterpAlpha) * MovementDirectionInterpStart + MovementDirectionInterpAlpha * DesiredMovementDirection;
		}
	}

	if (ActualMovementDirection != sf::Vector2f(0.0f, 0.0f))
	{
		Position += ActualMovementDirection * MovementSpeed * DeltaTime;
		ClampVector2f(Position, LevelInfo->Boundaries);
	}
}

void Actor::UpdatePositionInQuadTree()
{
	if (QuadTreeUpdateCounter >= QuadTreeUpdateInterval)
	{
		LevelInfo->UpdatePositionInQuadTree(this);
		QuadTreeUpdateCounter -= QuadTreeUpdateInterval;
	}
}

void Actor::RetreatToHealZone()
{
	sf::Vector2f NewDesiredMovementDirection = LevelInfo->GetHealZonePosition(Team) - GetPosition() + MovementDirectionOffset;
	NormalizeVector2f(NewDesiredMovementDirection);
	SetDesiredMovementDirection(NewDesiredMovementDirection);
}

void Actor::StopMovement()
{
	if (DesiredMovementDirection != sf::Vector2f(0.0f, 0.0f))
		SetDesiredMovementDirection(sf::Vector2f(0.0f, 0.0f));
}

void Actor::GoTowardsNearestEnemy()
{
	sf::Vector2f NewDesiredMovementDirection = VectorTowardsEnemy + MovementDirectionOffset;
	NormalizeVector2f(NewDesiredMovementDirection);
	SetDesiredMovementDirection(NewDesiredMovementDirection);
}

void Actor::SetDesiredMovementDirection(const sf::Vector2f& NewDesiredMovementDireciton)
{
	DesiredMovementDirection = NewDesiredMovementDireciton;
	MovementDirectionInterpStart = ActualMovementDirection;
	MovementDirectionInterpAlpha = 0.0f;
	bInterpolateMovementDirection = true;
}

const sf::Vector2f& Actor::GetPosition() const
{
	return Position;
}

ETeam Actor::GetTeam() const
{
	return Team;
}

void Actor::SetNearestEnemy(Actor* NewNearestEnemy)
{
	NearestEnemy = NewNearestEnemy;
	Blackboard.SetBNearestEnemyIsSet(NearestEnemy != nullptr);
}

Actor* Actor::GetNearestEnemy() const
{
	return NearestEnemy;
}

void Actor::UpdateLastQuadTreePosition(const sf::Vector2f& NewPositionInQuadTree)
{
	LastQuadTreePosition = NewPositionInQuadTree;
}

const sf::Vector2f& Actor::GetLastQuadTreePosition() const
{
	return LastQuadTreePosition;
}

void Actor::Heal(float HPToHeal)
{
	HP = std::min(HP + HPToHeal, MaxHP);
	Blackboard.SetHP(HP);
}

void Actor::TryToShoot()
{
	if (ShotTimeCounter >= ShotInterval)
	{
		bShouldDrawLaser = true;
		NearestEnemy->TakeDamage(Damage);
		ShotTimeCounter = sf::Time::Zero;
	}
}

void Actor::TakeDamage(float DamageAmount)
{
	HP -= DamageAmount;
	Blackboard.SetHP(HP);

	if (HP <= 0.0f)
		LevelInfo->DestroyActor(this);
}

void Actor::GenerateRandomMovementDirection(EDirection DirectionToAvoid)
{
	switch (DirectionToAvoid)
	{
	case EDirection::UP:
		DesiredMovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		DesiredMovementDirection.y = GetRandomFloat();
		break;
	case EDirection::RIGHT:
		DesiredMovementDirection.x = GetRandomFloat() - 1.0f;
		DesiredMovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	case EDirection::DOWN:
		DesiredMovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		DesiredMovementDirection.y = GetRandomFloat() - 1.0f;
		break;
	case EDirection::LEFT:
		DesiredMovementDirection.x = GetRandomFloat();
		DesiredMovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	case EDirection::NONE:
		DesiredMovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		DesiredMovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	}

	NormalizeVector2f(DesiredMovementDirection);
}
