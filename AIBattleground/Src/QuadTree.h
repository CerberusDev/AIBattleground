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
		QTNode* Parent;
		QTNode* TopRightChild;
		QTNode* TopLeftChild;
		QTNode* BottomRightChild;
		QTNode* BottomLeftChild;
		std::vector<Actor*> Actors;

	public:
		QTNode(QTNode* argParent, sf::Vector2f argCoords, sf::Vector2f argRegionSize) : Coords(argCoords), RegionSize(argRegionSize),
			Parent(argParent), TopRightChild(nullptr), TopLeftChild(nullptr), BottomRightChild(nullptr), BottomLeftChild(nullptr) {};
		~QTNode() 
		{
			if (TopRightChild)
				DeleteChildren();
		}

		void DeleteChildren()
		{
			delete TopRightChild;
			delete TopLeftChild;
			delete BottomRightChild;
			delete BottomLeftChild;

			TopRightChild = nullptr;
			TopLeftChild = nullptr;
			BottomRightChild = nullptr;
			BottomLeftChild = nullptr;
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
			TopRightChild = new QTNode(this, sf::Vector2f(Coords.x + RegionSize.x / 4.0f, Coords.y - RegionSize.y / 4.0f), RegionSize / 2.0f);
			TopLeftChild = new QTNode(this, sf::Vector2f(Coords.x - RegionSize.x / 4.0f, Coords.y - RegionSize.y / 4.0f), RegionSize / 2.0f);
			BottomRightChild = new QTNode(this, sf::Vector2f(Coords.x + RegionSize.x / 4.0f, Coords.y + RegionSize.y / 4.0f), RegionSize / 2.0f);
			BottomLeftChild = new QTNode(this, sf::Vector2f(Coords.x - RegionSize.x / 4.0f, Coords.y + RegionSize.y / 4.0f), RegionSize / 2.0f);

			for (auto it = Actors.begin(); it != Actors.end(); ++it)
				AddActor(*it);

			Actors.clear();
		}

		void RemoveActor(Actor* ActorToRemove)
		{
			if (TopRightChild)
			{
				sf::Vector2f ActorPosition = ActorToRemove->GetLastQuadTreePosition();

				if (ActorPosition.x > Coords.x)
				{
					if (ActorPosition.y > Coords.y)
						BottomLeftChild->RemoveActor(ActorToRemove);
					else
						TopLeftChild->RemoveActor(ActorToRemove);
				}
				else
				{
					if (ActorPosition.y > Coords.y)
						BottomRightChild->RemoveActor(ActorToRemove);
					else
						TopRightChild->RemoveActor(ActorToRemove);
				}
			}
			else
			{
				auto it = std::find(Actors.begin(), Actors.end(), ActorToRemove);
				Actors.erase(it);

				if (Actors.size() == 0 && Parent)
					Parent->MergeIfNecessary();
			}
		}

		void MergeIfNecessary()
		{
			if (TopRightChild->TopRightChild == nullptr && TopLeftChild->TopRightChild == nullptr
				&& BottomRightChild->TopRightChild == nullptr && BottomLeftChild->TopRightChild == nullptr)
			{
				int ActorsCount = TopRightChild->Actors.size() + TopLeftChild->Actors.size() + BottomRightChild->Actors.size() + BottomLeftChild->Actors.size();

				if (ActorsCount < 4)
				{
					Actors.insert(Actors.end(), TopRightChild->Actors.begin(), TopRightChild->Actors.end());
					Actors.insert(Actors.end(), TopLeftChild->Actors.begin(), TopLeftChild->Actors.end());
					Actors.insert(Actors.end(), BottomRightChild->Actors.begin(), BottomRightChild->Actors.end());
					Actors.insert(Actors.end(), BottomLeftChild->Actors.begin(), BottomLeftChild->Actors.end());

					DeleteChildren();
				}
			}
		}
	};

	QTNode* Root;

public:
	QuadTree(sf::Vector2f FirstNodeCoords);
	~QuadTree();

	void AddActor(Actor* NewActor);
	void RemoveActor(Actor* ActorToRemove);
};
