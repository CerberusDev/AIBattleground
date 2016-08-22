#include "Actor.h"
#include "TextureManager.h"

Actor::Actor(TextureManager* TexManager, const std::string& TexName, const sf::Vector2f& InitialPosition)
{
	Size = TexManager->InitTexture(&MySprite, TexName);
	Transform.translate(InitialPosition);
	MySprite.setOrigin(Size.x / 2.0f, Size.y / 2.0f);
}

Actor::~Actor()
{

}

void Actor::Draw(sf::RenderWindow* Window) const
{
	Window->draw(MySprite, Transform);
}
