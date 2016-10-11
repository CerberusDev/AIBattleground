// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "CapturePoint.h"
#include "TextureManager.h"

CapturePoint::CapturePoint(class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam) :
Position(argPosition), Team(argTeam)
{
	sf::Vector2u Size = TexManager->InitTexture(&CapturePointSprite, std::string("CapturePoint") + (Team == ETeam::TEAM_A ? "A" : "B") + "1");
	CapturePointSprite.setPosition(Position);
	CapturePointSprite.setOrigin(Size.x / 2.0f, Size.y / 2.0f);
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
	Window->draw(CapturePointSprite);
}
