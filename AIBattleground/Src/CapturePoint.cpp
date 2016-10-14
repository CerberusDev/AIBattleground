// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "CapturePoint.h"
#include "TextureManager.h"
#include "LevelInfo.h"

#include <iostream>

CapturePoint::CapturePoint(class LevelInfo* argLevelInfo, class TextureManager* TexManager, sf::Vector2f argPosition, ETeam argTeam) :
MyLevelInfo(argLevelInfo), ActorsArray(MyLevelInfo->GetActorsArray()), ActorsNumber(MyLevelInfo->GetActorsNumber()),
Position(argPosition), Team(argTeam), MaxHP(10000.0f), HP(MaxHP), HPPerSecond(500.0f), LowHPThreshold(MaxHP / 2.0f), Size(0.0f), bHasLowHP(false)
{
	sf::Vector2u FirstTextureSize;

	for (int i = 0; i < CAPTURE_POINT_SPRITES_NUMBER; ++i)
	{
		std::string TexName(std::string("CapturePoint") + (Team == ETeam::TEAM_A ? "A" : "B") + std::to_string(i + 1));
		sf::Vector2u TexSize = TexManager->InitTexture(&CapturePointSprite[i], TexName);

		if (i == 0)
			FirstTextureSize = TexSize;
		else if (TexSize != FirstTextureSize)
			std::cout << "CapturePoint construction: sprite textures in different sizes! " << TexName << std::endl;

		CapturePointSprite[i].setPosition(Position);
		CapturePointSprite[i].setOrigin(TexSize.x / 2.0f, TexSize.y / 2.0f);
	}

	Size = (float)FirstTextureSize.x;
	bHasLowHP = HP < LowHPThreshold;
}

CapturePoint::~CapturePoint()
{

}

void CapturePoint::Update(const float DeltaTime)
{
	ChangeHP(HPPerSecond * DeltaTime);
}

void CapturePoint::TakeDamage(float DamageAmount)
{
	ChangeHP(-DamageAmount);
}

void CapturePoint::ChangeHP(float HPDelta)
{
	bool bHadLowHP = bHasLowHP;
	HP = Clamp(HP + HPDelta, 0.0f, MaxHP);
	bHasLowHP = HP < LowHPThreshold;

	if (bHadLowHP != bHasLowHP)
	{
		for (int i = 0; i < ActorsNumber; ++i)
		{
			if (Actor* CurrActor = ActorsArray[i])
			{
				if (CurrActor->GetTeam() != Team)
					CurrActor->SetBEnemyCapturePointAtLowHP(bHasLowHP);
				else
					CurrActor->SetBAlliedCapturePointAtLowHP(bHasLowHP);
			}
		}
	}
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

sf::Vector2f CapturePoint::GetPosition() const
{
	return Position;
}

float CapturePoint::GetSize() const
{
	return Size;
}

bool CapturePoint::HasLowHP() const
{
	return bHasLowHP;
}
