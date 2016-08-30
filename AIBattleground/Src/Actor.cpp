// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Actor.h"
#include "TextureManager.h"
#include "LevelInfo.h"

Actor::Actor(class LevelInfo* argLevelInfo, TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition) :
LevelInfo(argLevelInfo), NearestEnemy(nullptr), Position(InitialPosition), DesiredMovementDirection(0.0f, 0.0f), 
ActualMovementDirection(0.0f, 0.0f), MovementSpeed(100.0f), DirectionChangeSpeed(5.0f), Team(argTeam), 
MovementDirectionInterpStart(0.0f, 0.0f), bInterpolateMovementDirection(false), MovementDirectionInterpAlpha(0.0f),
ShotInterval(sf::seconds(1.0f)), ShotTimeCounter(ShotInterval)
{
	Size = TexManager->InitTexture(&MySprite, TexName);
	MySprite.setOrigin(Size.x / 2.0f, Size.y / 2.0f);
}

Actor::~Actor()
{

}

void Actor::Draw(sf::RenderWindow* Window) const
{
	sf::Transform CurrTransform;
	CurrTransform.translate(Position);

	Window->draw(MySprite, CurrTransform);
}

void Actor::Update(const float DeltaTime)
{
	ShotTimeCounter += sf::seconds(DeltaTime);

	if (NearestEnemy)
	{
		sf::Vector2f DiffVec = NearestEnemy->GetPosition() - GetPosition();
		
		if (GetLength(DiffVec) > 100.0f)
		{
			DesiredMovementDirection = DiffVec;
			NormalizeVector2f(DesiredMovementDirection);
		}
		else
		{
			DesiredMovementDirection = sf::Vector2f(0.0f, 0.0f);
			TryToShoot();
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
		LevelInfo->DestroyActor(NearestEnemy);
		SetNearestEnemy(nullptr);
		ShotTimeCounter = sf::Time::Zero;
	}
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
