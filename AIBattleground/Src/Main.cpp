#include <SFML\Graphics.hpp>
#include <iostream>

#include "TextureManager.h"
#include "Actor.h"

#define ACTORS_AMOUNT 10000
#define RES_X 1000
#define RES_Y 800

int main(int argc, char** argv)
{
	std::srand((unsigned int)std::time(0));
	
	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 0;
	sf::RenderWindow Window(sf::VideoMode(RES_X, RES_Y), "SFML window title", sf::Style::Titlebar | sf::Style::Close, Settings);

	sf::Clock MainClock;
	sf::Time MainTimeCounter;
	int MainFPSCounter = 0;

	TextureManager TextureManager;

	Actor* Actors[ACTORS_AMOUNT];

	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		Actors[i] = new Actor(&TextureManager, "TestTex32a", sf::Vector2f((float)(std::rand() % RES_X), (float)(std::rand() % RES_Y)));

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}
		
		Window.clear(sf::Color(180, 180, 180));

		for (const Actor* CurrActor : Actors)
			CurrActor->Draw(&Window);

		Window.display();

		MainTimeCounter += MainClock.restart();
		MainFPSCounter++;

		if (MainTimeCounter.asSeconds() > 1.0f)
		{
			std::cout << "FPS: " << MainFPSCounter << std::endl;

			MainTimeCounter -= sf::seconds(1.0f);
			MainFPSCounter = 0;
		}
	}

	for (int i = 0; i < ACTORS_AMOUNT; ++i)
		delete Actors[i];

	return 0;
}
