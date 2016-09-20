// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <SFML\Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <thread>

#include "TextureManager.h"
#include "Actor.h"
#include "LevelInfo.h"

#define RES_X 1000
#define RES_Y 600

const float MaxDeltaTime = 0.2f;
const sf::Time FixedDeltaTime = sf::seconds(0.01666666667f);

int MainThreadFrameNum = 0;
sf::Clock MainClock;
sf::Time DrawDurationTimeCounter;

void main_RenderingThread(sf::RenderWindow* Window, LevelInfo* LevelInfo)
{
	int RenderingThreadFrameNum = 1;

	while (Window->isOpen())
	{
		if (RenderingThreadFrameNum <= MainThreadFrameNum)
		{
			sf::Time DrawStartTime = MainClock.getElapsedTime();

			Window->clear();
			LevelInfo->Draw(Window);
			Window->display();
			
			DrawDurationTimeCounter += MainClock.getElapsedTime() - DrawStartTime;

			++RenderingThreadFrameNum;
		}
		else
		{
			sf::sleep(sf::seconds(0.001f));
		}
	}

	std::cout << "Terminating rendering thread." << RenderingThreadFrameNum << std::endl;
}

int main()
{
	std::cout << "AI Battleground: Start! " << std::endl;

	std::srand((unsigned int)std::time(0));
	
	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 0;
	sf::RenderWindow Window(sf::VideoMode(RES_X, RES_Y), "AI Battleground", sf::Style::Titlebar | sf::Style::Close, Settings);

	TextureManager TextureManager;
	LevelInfo LevelInfo(&TextureManager, sf::FloatRect(0, 0, RES_X, RES_Y));

	sf::Time MainTimeCounter;
	sf::Time UpdateDurationTimeCounter;
	sf::Time DeltaTime;
	int MainFPSCounter = 0;

	Window.setActive(false);
	std::thread RenderingThread(main_RenderingThread, &Window, &LevelInfo);

	std::cout << "Initialization completed." << std::endl;

	while (Window.isOpen())
	{
		DeltaTime = MainClock.restart();
		MainTimeCounter += DeltaTime;
		MainFPSCounter++;

		while (MainTimeCounter.asSeconds() >= 1.0f)
		{
			std::cout << "-----------------------------------------------------" << std::endl;
			std::cout << "FPS: " << MainFPSCounter 
				<< std::setprecision(1) << std::fixed << "   Avg draw time: "
				<< DrawDurationTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms   Avg update time: "
				<< UpdateDurationTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms" << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;

			std::cout << "Finding nearest enemy: " << LevelInfo.T1.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			std::cout << "Heal zone update:      " << LevelInfo.T2.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			std::cout << "General actor update:  " << LevelInfo.T3.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			std::cout << "Update AI system:      " << LevelInfo.T4.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			std::cout << "Sync draw data:        " << LevelInfo.T5.asSeconds() * 1000.0f / MainFPSCounter << std::endl;

			LevelInfo.T1 = sf::Time::Zero;
			LevelInfo.T2 = sf::Time::Zero;
			LevelInfo.T3 = sf::Time::Zero;
			LevelInfo.T4 = sf::Time::Zero;
			LevelInfo.T5 = sf::Time::Zero;

			DrawDurationTimeCounter = sf::seconds(0.0f);
			UpdateDurationTimeCounter = sf::seconds(0.0f);
			MainTimeCounter -= sf::seconds(1.0f);
			MainFPSCounter = 0;
		}

		sf::Event Event;
		while (Window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
				Window.close();
		}

		//------------------- Update ----------------------
		sf::Time UpdateStartTime = MainClock.getElapsedTime();

		LevelInfo.Update(std::min(std::max(FixedDeltaTime.asSeconds(), DeltaTime.asSeconds()), MaxDeltaTime), MainTimeCounter);

		UpdateDurationTimeCounter += MainClock.getElapsedTime() - UpdateStartTime;

		//----------------- Time padding -------------------
		sf::Time BonusTime = FixedDeltaTime - MainClock.getElapsedTime();

		if ((BonusTime) > sf::Time::Zero)
			sf::sleep(BonusTime);

		++MainThreadFrameNum;
	}
	
	RenderingThread.join();
	return 0;
}
