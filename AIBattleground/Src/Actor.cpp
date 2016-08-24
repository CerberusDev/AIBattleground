#include "Actor.h"
#include "TextureManager.h"

Actor::Actor(TextureManager* TexManager, const std::string& TexName, const sf::Vector2f& InitialPosition) :
Speed(100.0f), MovementDirection(0.0f, 0.0f)
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

	if (Position.x > 1000.0f - Size.x / 2.0f)
		GenerateRandomMovementDirection(Direction::RIGHT);
	else if (Position.x < Size.x / 2.0f)
		GenerateRandomMovementDirection(Direction::LEFT);
	if (Position.y > 800.0f - Size.y / 2.0f)
		GenerateRandomMovementDirection(Direction::DOWN);
	else if (Position.y < Size.y / 2.0f)
		GenerateRandomMovementDirection(Direction::UP);
}

void Actor::GenerateRandomMovementDirection(Direction DirectionToAvoid)
{
	switch (DirectionToAvoid)
	{
	case Direction::UP:
		MovementDirection.x = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		MovementDirection.y = (float)std::rand() / (float)RAND_MAX;
		break;
	case Direction::RIGHT:
		MovementDirection.x = (float)std::rand() / (float)RAND_MAX - 1.0f;
		MovementDirection.y = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		break;
	case Direction::DOWN:
		MovementDirection.x = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		MovementDirection.y = (float)std::rand() / (float)RAND_MAX - 1.0f;
		break;
	case Direction::LEFT:
		MovementDirection.x = (float)std::rand() / (float)RAND_MAX;
		MovementDirection.y = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		break;
	case Direction::NONE:
		MovementDirection.x = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		MovementDirection.y = (float)std::rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
		break;
	}

	NormalizeVector2f(MovementDirection);
}