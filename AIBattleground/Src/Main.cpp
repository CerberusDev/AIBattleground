// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <SFML\Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <fstream>
#include <ctime>
#include <Windows.h>

#include "TextureManager.h"
#include "Actor.h"
#include "LevelInfo.h"

#define RES_X 1000
#define RES_Y 600

const sf::Time MaxDeltaTime = sf::seconds(0.2f);
const sf::Time FixedDeltaTime = sf::seconds(0.016f);

std::atomic<int> MainThreadFrameNum = 0;
std::atomic<float> DrawDurationTimeCounter;

void main_RenderingThread(sf::RenderWindow* Window, LevelInfo* LevelInfo)
{
	sf::Clock RenderingThreadClock;

	int RenderingThreadFrameNum = 1;

	while (Window->isOpen())
	{
		if (RenderingThreadFrameNum <= MainThreadFrameNum)
		{
			sf::Time DrawStartTime = RenderingThreadClock.getElapsedTime();

			Window->clear();
			LevelInfo->Draw(Window);
			Window->display();
			
			DrawDurationTimeCounter = DrawDurationTimeCounter + (RenderingThreadClock.getElapsedTime() - DrawStartTime).asSeconds();

			++RenderingThreadFrameNum;
		}
		else
		{
			sf::sleep(sf::seconds(0.001f));
		}
	}

	//std::cout << "Terminating rendering thread." << RenderingThreadFrameNum << std::endl;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	//std::cout << "AI Battleground: Start! " << std::endl;

	std::srand((unsigned int)std::time(0));
	
	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 0;
	sf::RenderWindow Window(sf::VideoMode(RES_X, RES_Y), "AI Battleground", sf::Style::Titlebar | sf::Style::Close, Settings);

	TextureManager TextureManager;
	LevelInfo LevelInfo(&TextureManager, sf::FloatRect(0, 0, RES_X, RES_Y));

	sf::Clock MainClock;
	sf::Time MainTimeCounter;
	sf::Time ExcessingTime;
	sf::Time UpdateDurationTimeCounter;
	sf::Time DataSyncTimeCounter;
	sf::Time DeltaTime;
	sf::Time TimeFromStart;
	int MainFPSCounter = 0;

	std::ofstream LogFile;
	std::time_t t = time(0);
	struct tm* now = localtime(&t);
	LogFile.open(std::string("Logs/Log FSMAlways ") + std::to_string(now->tm_mday) + "-" + std::to_string(1 + now->tm_mon) + " " 
		+ std::to_string(now->tm_hour) + "." + std::to_string(now->tm_min) + "." + std::to_string(now->tm_sec) + ".txt");

	Window.setActive(false);
	std::thread RenderingThread(main_RenderingThread, &Window, &LevelInfo);

	//std::cout << "Initialization completed." << std::endl;

	int XYZ = 0;

	while (Window.isOpen())
	{
		DeltaTime = MainClock.restart();
		MainTimeCounter += DeltaTime;
		TimeFromStart += DeltaTime;
		MainFPSCounter++;

		if (MainTimeCounter.asSeconds() >= 1.0f)
		{
			//std::cout << "-----------------------------------------------------" << std::endl;
			//std::cout << "FPS: " << MainFPSCounter 
			//	<< std::setprecision(1) << std::fixed << "   Avg draw t: "
			//	<< DrawDurationTimeCounter * 1000.0f / MainFPSCounter << " ms   Avg update t: "
			//	<< UpdateDurationTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms   Avg sync t: " 
			//	<< DataSyncTimeCounter.asSeconds() * 1000.0f / MainFPSCounter << " ms" << std::endl;
			//std::cout << "-----------------------------------------------------" << std::endl;

			//std::cout << "Finding nearest enemy: " << LevelInfo.T1.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			//std::cout << "Level objects update:  " << LevelInfo.T2.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			//std::cout << "General actor update:  " << LevelInfo.T3.asSeconds() * 1000.0f / MainFPSCounter << std::endl;
			//std::cout << "Update AI system:      " << LevelInfo.T4.asSeconds() * 1000.0f / MainFPSCounter << std::endl;

			//LevelInfo.T1 = sf::Time::Zero;
			//LevelInfo.T2 = sf::Time::Zero;
			//LevelInfo.T3 = sf::Time::Zero;
			//LevelInfo.T4 = sf::Time::Zero;

			if (TimeFromStart > sf::seconds(15.0f))
			{
				++XYZ;
				LogFile << LevelInfo.T4.asMicroseconds() / MainFPSCounter << std::endl;
				LevelInfo.T4 = sf::Time::Zero;

				if (XYZ == 30)
					Window.close();
			}

			//DrawDurationTimeCounter = 0.0f;
			//UpdateDurationTimeCounter = sf::Time::Zero;
			//DataSyncTimeCounter = sf::Time::Zero;
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

		LevelInfo.Update(std::min(std::max(FixedDeltaTime.asSeconds(), DeltaTime.asSeconds()), MaxDeltaTime.asSeconds()), FixedDeltaTime);

		UpdateDurationTimeCounter += MainClock.getElapsedTime() - UpdateStartTime;

		//------------------- Data sync ----------------------

		sf::Time SyncStartTime = MainClock.getElapsedTime();

		LevelInfo.SyncData();

		DataSyncTimeCounter += MainClock.getElapsedTime() - SyncStartTime;

		//--------------------------------------------------

		//if (TimeFromStart > sf::seconds(15.0f))
		//{
		//	LogFile << LevelInfo.T4.asMicroseconds() << std::endl;
		//}

		//LevelInfo.T4 = sf::Time::Zero;


		//----------------- Time padding -------------------
		sf::Time BonusTime = FixedDeltaTime - MainClock.getElapsedTime();

		if ((BonusTime) > sf::Time::Zero)
		{
			if (BonusTime > ExcessingTime)
			{
				sf::Time TimeToSleep = BonusTime - ExcessingTime;
				ExcessingTime = sf::Time::Zero;
				sf::sleep(TimeToSleep);
			}
			else
			{
				ExcessingTime += -BonusTime;
			}
		}
		else
		{
			ExcessingTime += -BonusTime;

			if (ExcessingTime > MaxDeltaTime)
				ExcessingTime = MaxDeltaTime;
		}

		++MainThreadFrameNum;
	}
	
	LogFile.close();

	RenderingThread.join();
	return 0;
}
