// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "HealZone.h"
#include "TextureManager.h"
#include "Actor.h"

HealZone::HealZone(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam, class Actor** argActorsArray, int argActorsNumber) :
Position(argPosition), Team(argTeam), HealTimeCounter(sf::Time::Zero), HealTimeStep(sf::seconds(0.1f)), HPToHeal(10.f), 
ActorsArray(argActorsArray), ActorsNumber(argActorsNumber)
{
	sf::Vector2u Size = TexManager->InitTexture(&HealZoneSprite, std::string("HealZone") + (Team == ETeam::TEAM_A ? "A" : "B"));
	Radius = Size.x / 2.0f;
	SquaredRadius = std::pow(Radius, 2);
	HealZoneSprite.setPosition(Position);
	HealZoneSprite.setOrigin(Radius, Radius);
}

HealZone::~HealZone()
{

}

sf::Vector2f HealZone::GetPosition() const
{
	return Position;
}

void HealZone::Update(const float DeltaTime)
{
	HealTimeCounter += sf::seconds(DeltaTime);

	if (HealTimeCounter > HealTimeStep)
	{
		for (int i = 0; i < ActorsNumber; ++i)
			if (ActorsArray[i] && ActorsArray[i]->GetTeam() == Team && GetSquaredDist(ActorsArray[i]->GetPosition(), Position) < SquaredRadius)
				ActorsArray[i]->Heal(HPToHeal);

		HealTimeCounter -= HealTimeStep;
	}
}

void HealZone::Draw(sf::RenderWindow* Window) const
{
	Window->draw(HealZoneSprite);
}
