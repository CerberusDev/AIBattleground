// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <SFML\Graphics.hpp>
#include <iostream>
#include <iomanip>

#include "TextureManager.h"
#include "Actor.h"
#include "LevelInfo.h"

#define RES_X 1000
#define RES_Y 800

int main()
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
	sf::Time UpdateTimeCounter;
	sf::Time DrawTimeCounter;
	sf::Time DeltaTime;
	int MainFPSCounter = 0;

	std::cout << "Initialization completed." << std::endl;

	while (Window.isOpen())
	{
		DeltaTime = MainClock.restart();
		MainTimeCounter += DeltaTime;
		MainFPSCounter++;

		while (MainTimeCounter.asSeconds() >= 1.0f)
		{
			std::cout << "FPS: " << MainFPSCounter 
				<< std::setprecision(1) << std::fixed << "   Avg draw time: "
				<< DrawTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms   Avg update time: " 
				<< UpdateTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms" << std::endl;

			UpdateTimeCounter = sf::seconds(0.0f);
			DrawTimeCounter = sf::seconds(0.0f);
			MainTimeCounter -= sf::seconds(1.0f);
			MainFPSCounter = 0;
		}

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}

		sf::Clock UpdateClock;

		LevelInfo.Update(DeltaTime.asSeconds(), MainTimeCounter);

		UpdateTimeCounter += UpdateClock.getElapsedTime();

		sf::Clock DrawClock;

		Window.clear();
		LevelInfo.Draw(&Window);
		Window.display();

		DrawTimeCounter += DrawClock.getElapsedTime();
	}

	return 0;
}
