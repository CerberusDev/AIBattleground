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
MovementDirectionInterpAlpha(0.0f), bDrawBeam(false), ShotInterval(sf::seconds(0.5f)), ShotTimeCounter(ShotInterval)
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

	BeamTexSize = TexManager->InitTexture(&BeamSprite, "LaserBeam");
	BeamSprite.setOrigin(0.0f, BeamTexSize.y / 2.0f);

	MovementDirectionOffset.x = GetRandomFloat(ShotDist) - ShotDist / 2.0f;
	MovementDirectionOffset.y = GetRandomFloat(ShotDist) - ShotDist / 2.0f;

	MovementStopOffset = GetRandomFloat(ShotDist * 0.4f);
}

Actor::~Actor()
{

}

void Actor::DrawRobot(sf::RenderWindow* Window) const
{
	sf::Transform CurrTransform;
	CurrTransform.translate(Position);

	Window->draw(GetRobotSprite(), CurrTransform);
}

const sf::Sprite& Actor::GetRobotSprite() const
{
	const float HPRatio = HP / MaxHP;
	return RobotSprite[(int)((1.0f - HPRatio) * ROBOT_SPRITES_AMOUNT)];
}

void Actor::DrawBeam(sf::RenderWindow* Window) const
{
	if (bDrawBeam)
	{
		sf::Vector2f Diff = VectorTowardsEnemy;
		NormalizeVector2f(Diff);
		float Angle = acos(Diff.x);
		Angle *= (float)M_1_PI * 180.0f;

		if (Diff.y < 0.0f)
			Angle = 360.0f - Angle;

		sf::Transform CurrTransform;
		CurrTransform.translate(Position);
		CurrTransform.rotate(Angle);
		CurrTransform.scale(GetLength(VectorTowardsEnemy) / BeamTexSize.x, 1.0f);

		Window->draw(BeamSprite, CurrTransform);
		bDrawBeam = false;
	}
}

void Actor::Update(const float DeltaTime)
{
	ShotTimeCounter += sf::seconds(DeltaTime);

	if (NearestEnemy)
	{
		VectorTowardsEnemy = NearestEnemy->GetPosition() - GetPosition();
		float VectorTowardsEnemyLength = GetLength(VectorTowardsEnemy);

		if (VectorTowardsEnemyLength < ShotDist)
			TryToShoot();

		if (VectorTowardsEnemyLength > ShotDist - MovementStopOffset)
		{
			DesiredMovementDirection = VectorTowardsEnemy + MovementDirectionOffset;
			NormalizeVector2f(DesiredMovementDirection);
		}
		else
		{
			DesiredMovementDirection = sf::Vector2f(0.0f, 0.0f);
		}

		MovementDirectionInterpStart = ActualMovementDirection;
		MovementDirectionInterpAlpha = 0.0f;
		bInterpolateMovementDirection = true;
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
	{
		DesiredMovementDirection = sf::Vector2f(0.0f, 0.0f);
		MovementDirectionInterpStart = ActualMovementDirection;
		MovementDirectionInterpAlpha = 0.0f;
		bInterpolateMovementDirection = true;
	}
}

Actor* Actor::GetNearestEnemy() const
{
	return NearestEnemy;
}

void Actor::TryToShoot()
{
	if (ShotTimeCounter >= ShotInterval)
	{
		bDrawBeam = true;
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
