// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Actor.h"
#include "TextureManager.h"
#include "LevelInfo.h"

Actor::Actor(class LevelInfo* argLevelInfo, TextureManager* TexManager, const std::string& TexName, const ETeam argTeam, const sf::Vector2f& InitialPosition) :
LevelInfo(argLevelInfo), NearestEnemy(nullptr), Position(InitialPosition), MovementDirection(0.0f, 0.0f), Speed(100.0f), Team(argTeam)
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
	if (NearestEnemy)
	{
		MovementDirection = NearestEnemy->GetPosition() - GetPosition();
		NormalizeVector2f(MovementDirection);
	}

	Position += MovementDirection * Speed * DeltaTime;
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
