// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

#include "Actor.h"

#define MIN_REGION_SIZE 13.0f

class QuadTree
{
private:
	class QTNode
	{
	private:
		sf::Vector2f Coords;
		sf::Vector2f RegionSize;
		QTNode* TopRightChild;
		QTNode* TopLeftChild;
		QTNode* BottomRightChild;
		QTNode* BottomLeftChild;
		std::vector<Actor*> Actors;

	public:
		QTNode(sf::Vector2f argCoords, sf::Vector2f argRegionSize) : Coords(argCoords), RegionSize(argRegionSize),
			TopRightChild(nullptr), TopLeftChild(nullptr), BottomRightChild(nullptr), BottomLeftChild(nullptr) {};
		~QTNode() 
		{
			if (TopRightChild)
			{
				delete TopRightChild;
				delete TopLeftChild;
				delete BottomRightChild;
				delete BottomLeftChild;
			}
		}

		void AddActor(Actor* NewActor)
		{
			if (TopRightChild)
			{
				sf::Vector2f ActorPosition = NewActor->GetPosition();

				if (ActorPosition.x > Coords.x)
				{
					if (ActorPosition.y > Coords.y)
						BottomLeftChild->AddActor(NewActor);
					else
						TopLeftChild->AddActor(NewActor);
				}
				else
				{
					if (ActorPosition.y > Coords.y)
						BottomRightChild->AddActor(NewActor);
					else
						TopRightChild->AddActor(NewActor);
				}
			}
			else
			{
				Actors.push_back(NewActor);

				if (RegionSize.x / 2.0f > MIN_REGION_SIZE && RegionSize.y / 2.0f > MIN_REGION_SIZE && Actors.size() > 5)
					Subdivide();
			}
		}

		void Subdivide()
		{
			TopRightChild = new QTNode(sf::Vector2f(Coords.x + RegionSize.x / 4.0f, Coords.y - RegionSize.y / 4.0f), RegionSize / 2.0f);
			TopLeftChild = new QTNode(sf::Vector2f(Coords.x - RegionSize.x / 4.0f, Coords.y - RegionSize.y / 4.0f), RegionSize / 2.0f);
			BottomRightChild = new QTNode(sf::Vector2f(Coords.x + RegionSize.x / 4.0f, Coords.y + RegionSize.y / 4.0f), RegionSize / 2.0f);
			BottomLeftChild = new QTNode(sf::Vector2f(Coords.x - RegionSize.x / 4.0f, Coords.y + RegionSize.y / 4.0f), RegionSize / 2.0f);

			for (auto it = Actors.begin(); it != Actors.end(); ++it)
				AddActor(*it);

			Actors.clear();
		}
	};

	QTNode* Root;

public:
	QuadTree(sf::Vector2f FirstNodeCoords);
	~QuadTree();

	void AddActor(Actor* NewActor);
};
