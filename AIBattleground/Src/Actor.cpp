// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Actor.h"
#include "TextureManager.h"
#include "LevelInfo.h"

Actor::Actor(class LevelInfo* argLevelInfo, TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition) :
LevelInfo(argLevelInfo), MovementDirection(0.0f, 0.0f), Speed(100.0f), Team(argTeam)
{
	Size = TexManager->InitTexture(&MySprite, TexName);
	Position = InitialPosition;
	MySprite.setOrigin(Size.x / 2.0f, Size.y / 2.0f);

	GenerateRandomMovementDirection();
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
	Position += MovementDirection * Speed * DeltaTime;

	if (Position.x > LevelInfo->RightBottomEdge.x - Size.x / 2.0f)
		GenerateRandomMovementDirection(EDirection::RIGHT);
	else if (Position.x < LevelInfo->Boundaries.left + Size.x / 2.0f)
		GenerateRandomMovementDirection(EDirection::LEFT);
	if (Position.y > LevelInfo->RightBottomEdge.y - Size.y / 2.0f)
		GenerateRandomMovementDirection(EDirection::DOWN);
	else if (Position.y < LevelInfo->Boundaries.top + Size.y / 2.0f)
		GenerateRandomMovementDirection(EDirection::UP);
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
}

void Actor::GenerateRandomMovementDirection(EDirection DirectionToAvoid)
{
	switch (DirectionToAvoid)
	{
	case EDirection::UP:
		MovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		MovementDirection.y = GetRandomFloat();
		break;
	case EDirection::RIGHT:
		MovementDirection.x = GetRandomFloat() - 1.0f;
		MovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	case EDirection::DOWN:
		MovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		MovementDirection.y = GetRandomFloat() - 1.0f;
		break;
	case EDirection::LEFT:
		MovementDirection.x = GetRandomFloat();
		MovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	case EDirection::NONE:
		MovementDirection.x = GetRandomFloat(2.0f) - 1.0f;
		MovementDirection.y = GetRandomFloat(2.0f) - 1.0f;
		break;
	}

	NormalizeVector2f(MovementDirection);
}
