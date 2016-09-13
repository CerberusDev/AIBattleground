// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "QuadTree.h"

QuadTree::QuadTree(sf::Vector2f FirstNodeCoords)
{
	Root = new QTNode(nullptr, FirstNodeCoords, FirstNodeCoords * 2.0f);
}

QuadTree::~QuadTree()
{
	delete Root;
}

void QuadTree::AddActor(Actor* NewActor)
{
	Root->AddActor(NewActor);
	NewActor->UpdateLastQuadTreePosition();
}

void QuadTree::RemoveActor(Actor* ActorToRemove)
{
	Root->RemoveActor(ActorToRemove);
}

Actor* QuadTree::FindNearestNeighborTo(sf::Vector2f BasePoint)
{
	Actor* ResultActor = nullptr;

	if (Root->TopRightChild || Root->Actors.size() > 0)
	{
		Actor* NearestNeighbor = Root->GetNaiveNeighbor(BasePoint);
		float SquaredDist = GetSquaredDist(BasePoint, NearestNeighbor->GetPosition());
		float Dist = std::sqrt(SquaredDist);
		Root->GetNearestNeighbor(BasePoint, NearestNeighbor, Dist, SquaredDist);
		ResultActor = NearestNeighbor;
	}
	
	return ResultActor;
}
