// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "HealZone.h"
#include "TextureManager.h"

HealZone::HealZone(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam) :
Position(argPosition), Team(argTeam)
{
	sf::Vector2u Size = TexManager->InitTexture(&HealZoneSprite, std::string("HealZone") + (Team == ETeam::TEAM_A ? "A" : "B"));
	Radius = Size.x / 2.0f;
	HealZoneSprite.setPosition(Position);
	HealZoneSprite.setOrigin(Radius, Radius);
}

HealZone::~HealZone()
{

}

void HealZone::Draw(sf::RenderWindow* Window) const
{
	Window->draw(HealZoneSprite);
}
