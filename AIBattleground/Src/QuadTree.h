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
	struct QTNode
	{
		sf::Vector2f Coords;
		sf::Vector2f RegionSize;
		QTNode* Parent;
		QTNode* TopRightChild;
		QTNode* TopLeftChild;
		QTNode* BottomRightChild;
		QTNode* BottomLeftChild;
		std::vector<Actor*> Actors;

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

		QTNode* SelectProperChild(sf::Vector2f Position)
		{
			QTNode* ResultNode = nullptr;

			if (Position.x > Coords.x)
			{
				if (Position.y > Coords.y)
					ResultNode = BottomRightChild;
				else
					ResultNode = TopRightChild;
			}
			else
			{
				if (Position.y > Coords.y)
					ResultNode = BottomLeftChild;
				else
					ResultNode = TopLeftChild;
			}

			return ResultNode;
		}

		void AddActor(Actor* NewActor)
		{
			if (TopRightChild)
			{
				SelectProperChild(NewActor->GetPosition())->AddActor(NewActor);
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
				SelectProperChild(ActorToRemove->GetLastQuadTreePosition())->RemoveActor(ActorToRemove);
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

					if (Actors.size() == 0 && Parent)
						Parent->MergeIfNecessary();
				}
			}
		}

		Actor* GetNaiveNeighbor(sf::Vector2f BasePoint)
		{
			if (TopRightChild)
			{
				QTNode* ProperChild = SelectProperChild(BasePoint);

				if (ProperChild->TopRightChild || ProperChild->Actors.size() > 0)
					return ProperChild->GetNaiveNeighbor(BasePoint);

				if (TopRightChild->TopRightChild || TopRightChild->Actors.size() > 0)
					return TopRightChild->GetNaiveNeighbor(BasePoint);

				if (TopLeftChild->TopRightChild || TopLeftChild->Actors.size() > 0)
					return TopLeftChild->GetNaiveNeighbor(BasePoint);

				if (BottomRightChild->TopRightChild || BottomRightChild->Actors.size() > 0)
					return BottomRightChild->GetNaiveNeighbor(BasePoint);

				return BottomLeftChild->GetNaiveNeighbor(BasePoint);
			}
			else
			{
				Actor* ResultActor = nullptr;
				float MinSquaredDist = FLT_MAX;

				for (auto it = Actors.begin(); it != Actors.end(); ++it)
				{
					float SquaredDist = GetSquaredDist((*it)->GetPosition(), BasePoint);

					if (SquaredDist < MinSquaredDist)
					{
						MinSquaredDist = SquaredDist;
						ResultActor = *it;
					}
				}

				return ResultActor;
			}
		}

		void GetNearestNeighbor(sf::Vector2f BasePoint, Actor*& CurrentNearestNeighbor, float& CurrentMinDist, float& CurrentMinDistSquared) const
		{
			sf::Vector2f Diff = Abs(Coords - BasePoint) - RegionSize;

			if (Diff.x < CurrentMinDist && Diff.y < CurrentMinDist)
			{
				if (TopRightChild)
				{
					TopRightChild->GetNearestNeighbor(BasePoint, CurrentNearestNeighbor, CurrentMinDist, CurrentMinDistSquared);
					TopLeftChild->GetNearestNeighbor(BasePoint, CurrentNearestNeighbor, CurrentMinDist, CurrentMinDistSquared);
					BottomRightChild->GetNearestNeighbor(BasePoint, CurrentNearestNeighbor, CurrentMinDist, CurrentMinDistSquared);
					BottomLeftChild->GetNearestNeighbor(BasePoint, CurrentNearestNeighbor, CurrentMinDist, CurrentMinDistSquared);
				}
				else
				{
					for (auto it = Actors.begin(); it != Actors.end(); ++it)
					{
						float SquaredDist = GetSquaredDist(BasePoint, (*it)->GetPosition());

						if (SquaredDist < CurrentMinDistSquared)
						{
							CurrentMinDist = std::sqrt(SquaredDist);
							CurrentMinDistSquared = SquaredDist;
							CurrentNearestNeighbor = *it;
						}
					}
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
	Actor* FindNearestNeighborTo(sf::Vector2f BasePoint);
};
