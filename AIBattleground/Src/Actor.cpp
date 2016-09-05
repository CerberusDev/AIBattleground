// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "Actor.h"
#include "TextureManager.h"
#include "LevelInfo.h"

Actor::Actor(class LevelInfo* argLevelInfo, TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition) :
LevelInfo(argLevelInfo), NearestEnemy(nullptr), Position(InitialPosition), DesiredMovementDirection(0.0f, 0.0f), 
ActualMovementDirection(0.0f, 0.0f), VectorTowardsEnemy(0.0f, 0.0f), ShotDist(75.0f), MovementSpeed(100.0f), DirectionChangeSpeed(5.0f),
MaxHP(100.0f), HP(MaxHP), Damage(10.0f), Team(argTeam), MovementDirectionInterpStart(0.0f, 0.0f), bInterpolateMovementDirection(false),
MovementDirectionInterpAlpha(0.0f), bDrawLaser(false), ShotInterval(sf::seconds(0.5f)), ShotTimeCounter(ShotInterval), 
bHealing(false)
{
	for (int i = 0; i < ROBOT_SPRITES_AMOUNT; ++i)
	{
		sf::Vector2u TexSize = TexManager->InitTexture(&RobotSprite[i], TexName + std::to_string(i + 1));

		if (i == 0)
			Size = TexSize;
		else if (TexSize != Size)
			std::cout << "Actor construction: sprite textures in different sizes! " << TexName << " " << i << std::endl;

		RobotSprite[i].setOrigin(Size.x / 2.0f, Size.y / 2.0f);
	}

	BeamTexSize = TexManager->InitTexture(&LaserBeamSprite, "LaserBeam");
	LaserBeamSprite.setOrigin(0.0f, BeamTexSize.y / 2.0f);

	sf::Vector2u BurstTexSize = TexManager->InitTexture(&LaserBurstSprite, "LaserBurst");
	LaserBurstSprite.setOrigin(BurstTexSize.x / 2.0f, BurstTexSize.y / 2.0f);

	MovementDirectionOffset.x = GetRandomFloat(ShotDist) - ShotDist / 2.0f;
	MovementDirectionOffset.y = GetRandomFloat(ShotDist) - ShotDist / 2.0f;

	MovementStopOffset = GetRandomFloat(ShotDist * 0.4f);
}

Actor::~Actor()
{

}

void Actor::DrawRobot(sf::RenderWindow* Window) const
{
	DrawLaserBurst(Window);

	sf::Transform CurrTransform;
	CurrTransform.translate(Position);

	Window->draw(GetRobotSprite(), CurrTransform);
}

const sf::Sprite& Actor::GetRobotSprite() const
{
	const float HPRatio = HP / MaxHP;
	return RobotSprite[(int)((1.0f - HPRatio) * ROBOT_SPRITES_AMOUNT)];
}

void Actor::DrawLaserBeam(sf::RenderWindow* Window) const
{
	if (bDrawLaser)
	{
		sf::Vector2f Diff = VectorTowardsEnemy;
		NormalizeVector2f(Diff);
		AngleToEnemy = acos(Diff.x);
		AngleToEnemy *= (float)M_1_PI * 180.0f;

		if (Diff.y < 0.0f)
			AngleToEnemy = 360.0f - AngleToEnemy;

		sf::Transform CurrTransform;
		CurrTransform.translate(Position);
		CurrTransform.rotate(AngleToEnemy);
		CurrTransform.scale(GetLength(VectorTowardsEnemy) / BeamTexSize.x, 1.0f);

		Window->draw(LaserBeamSprite, CurrTransform);
	}
}

void Actor::DrawLaserBurst(sf::RenderWindow* Window) const
{
	if (bDrawLaser)
	{
		sf::Transform CurrTransform;
		CurrTransform.translate(Position);
		CurrTransform.rotate(AngleToEnemy);
		CurrTransform.translate(sf::Vector2f(Size.x / 2.0f + 1.0f, 0.0f));
		Window->draw(LaserBurstSprite, CurrTransform);

		bDrawLaser = false;
	}
}

void Actor::Update(const float DeltaTime)
{
	ShotTimeCounter += sf::seconds(DeltaTime);

	if (bHealing)
	{
		if (GetSquaredDist(LevelInfo->GetHealZonePosition(Team) + MovementDirectionOffset, GetPosition()) > 100.0f)
		{
			RetreatToHealZone();
		}
		else if (DesiredMovementDirection != sf::Vector2f(0.0f, 0.0f))
		{
			SetDesiredMovementDirection(sf::Vector2f(0.0f, 0.0f));
		}
	}
	else
	{
		if (HP < MaxHP / 2.0f)
		{
			RetreatToHealZone();
		}
		else if (NearestEnemy)
		{
			VectorTowardsEnemy = NearestEnemy->GetPosition() - GetPosition();
			float VectorTowardsEnemyLength = GetLength(VectorTowardsEnemy);

			if (VectorTowardsEnemyLength > ShotDist - MovementStopOffset)
			{
				sf::Vector2f NewDesiredMovementDirection = VectorTowardsEnemy + MovementDirectionOffset;
				NormalizeVector2f(NewDesiredMovementDirection);
				SetDesiredMovementDirection(NewDesiredMovementDirection);
			}
			else
			{
				if (DesiredMovementDirection != sf::Vector2f(0.0f, 0.0f))
					SetDesiredMovementDirection(sf::Vector2f(0.0f, 0.0f));

				TryToShoot();
			}
		}
	}
	
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

	Position += ActualMovementDirection * MovementSpeed * DeltaTime;
	ClampVector2f(Position, LevelInfo->Boundaries);
}

void Actor::RetreatToHealZone()
{
	sf::Vector2f NewDesiredMovementDirection = LevelInfo->GetHealZonePosition(Team) - GetPosition() + MovementDirectionOffset;
	NormalizeVector2f(NewDesiredMovementDirection);
	SetDesiredMovementDirection(NewDesiredMovementDirection);
}

void Actor::SetDesiredMovementDirection(sf::Vector2f NewDesiredMovementDireciton)
{
	DesiredMovementDirection = NewDesiredMovementDireciton;
	MovementDirectionInterpStart = ActualMovementDirection;
	MovementDirectionInterpAlpha = 0.0f;
	bInterpolateMovementDirection = true;
}

sf::Vector2f Actor::GetPosition() const
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

	if (NearestEnemy == nullptr)
		SetDesiredMovementDirection(sf::Vector2f(0.0f, 0.0f));
}

Actor* Actor::GetNearestEnemy() const
{
	return NearestEnemy;
}

void Actor::Heal(float HPToHeal)
{
	if (HP < MaxHP)
	{
		bHealing = true;

		HP += HPToHeal;

		if (HP >= MaxHP)
		{
			HP = MaxHP;
			bHealing = false;
		}
	}
}

void Actor::TryToShoot()
{
	if (ShotTimeCounter >= ShotInterval)
	{
		bDrawLaser = true;
		NearestEnemy->TakeDamage(Damage);
		ShotTimeCounter = sf::Time::Zero;
	}
}

void Actor::TakeDamage(float DamageAmount)
{
	HP -= DamageAmount;

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
