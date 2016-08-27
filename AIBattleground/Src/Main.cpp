// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <SFML\Graphics.hpp>
#include <iostream>

#include "TextureManager.h"
#include "Actor.h"
#include "LevelInfo.h"

#define RES_X 1000
#define RES_Y 800

int main(int argc, char** argv)
{
	std::cout << "AI Battleground: Start! " << std::endl;

	std::srand((unsigned int)std::time(0));
	
	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 0;
	sf::RenderWindow Window(sf::VideoMode(RES_X, RES_Y), "AI Battleground", sf::Style::Titlebar | sf::Style::Close, Settings);

	TextureManager TextureManager;
	LevelInfo LevelInfo(&TextureManager, sf::FloatRect(0, 0, RES_X, RES_Y));

	sf::Clock MainClock;
	sf::Time MainTimeCounter;
	sf::Time DeltaTime;
	int MainFPSCounter = 0;

	std::cout << "Initialization completed." << std::endl;

	while (Window.isOpen())
	{
		DeltaTime = MainClock.restart();
		MainTimeCounter += DeltaTime;
		MainFPSCounter++;

		if (MainTimeCounter.asSeconds() > 1.0f)
		{
			std::cout << "FPS: " << MainFPSCounter << std::endl;

			MainTimeCounter -= sf::seconds(1.0f);
			MainFPSCounter = 0;
		}

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}

		LevelInfo.Update(DeltaTime.asSeconds());

		Window.clear();
		LevelInfo.Draw(&Window);
		Window.display();
	}

	return 0;
}
