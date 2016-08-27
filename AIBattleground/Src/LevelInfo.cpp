// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "LevelInfo.h"
#include "Actor.h"
#include "TextureManager.h"

LevelInfo::LevelInfo(class TextureManager* TexManager, const sf::FloatRect& LevelBoundaries) :
Boundaries(LevelBoundaries), RightBottomEdge(LevelBoundaries.left + LevelBoundaries.width, LevelBoundaries.top + LevelBoundaries.height)
{
	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		Actors[i] = new Actor(this, TexManager, "TestTex16a", sf::Vector2f((float)(std::rand() % (int)Boundaries.width), (float)(std::rand() % (int)Boundaries.height)));

	TexManager->InitTexture(&BackgroundSprite, "Background256", true);
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (int)Boundaries.width, (int)Boundaries.height));
}

LevelInfo::~LevelInfo()
{
	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		delete Actors[i];
}

void LevelInfo::Draw(sf::RenderWindow* Window) const
{
	Window->draw(BackgroundSprite);

	for (const Actor* CurrActor : Actors)
		CurrActor->Draw(Window);
}

void LevelInfo::Update(const float DeltaTime)
{
	for (Actor* CurrActor : Actors)
		CurrActor->Update(DeltaTime);
}
