// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "CapturePoint.h"
#include "TextureManager.h"

#include <iostream>

CapturePoint::CapturePoint(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam) :
Position(argPosition), Team(argTeam), MaxHP(1000.0f), HP(MaxHP)
{
	sf::Vector2u FirstTextureSize;

	for (int i = 0; i < CAPTURE_POINT_SPRITES_NUMBER; ++i)
	{
		std::string TexName(std::string("CapturePoint") + (Team == ETeam::TEAM_A ? "A" : "B") + std::to_string(i + 1));
		sf::Vector2u TexSize = TexManager->InitTexture(&CapturePointSprite[i], TexName);

		if (i == 0)
			FirstTextureSize = TexSize;
		else if (TexSize != FirstTextureSize)
			std::cout << "Actor construction: sprite textures in different sizes! " << TexName << std::endl;

		CapturePointSprite[i].setPosition(Position);
		CapturePointSprite[i].setOrigin(TexSize.x / 2.0f, TexSize.y / 2.0f);
	}
}

CapturePoint::~CapturePoint()
{

}

sf::Vector2f CapturePoint::GetPosition() const
{
	return Position;
}

void CapturePoint::Draw(sf::RenderWindow* Window) const
{
	Window->draw(GetCurrentSprite());
}

const sf::Sprite& CapturePoint::GetCurrentSprite() const
{
	const float HPRatio = HP / MaxHP;
	return CapturePointSprite[(int)((1.0f - HPRatio) * (CAPTURE_POINT_SPRITES_NUMBER - 1))];
}