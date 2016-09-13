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
	return QuickFindNearNeighborTo(BasePoint, true);
}

Actor* QuadTree::QuickFindNearNeighborTo(Actor* RequestingActor)
{
	Actor* ResultActor = QuickFindNearNeighborTo(RequestingActor->GetNearestEnemy()->GetPosition());

	if (ResultActor == nullptr)
		ResultActor = FindNearestNeighborTo(RequestingActor->GetPosition());

	return ResultActor;
}

Actor* QuadTree::QuickFindNearNeighborTo(sf::Vector2f BasePoint, bool bForceNearestNeighborSearch)
{
	Actor* ResultActor = nullptr;

	if (Root->TopRightChild || Root->Actors.size() > 0)
	{
		Actor* NearNeighbor = Root->GetNaiveNeighbor(BasePoint);
		float SquaredDist = GetSquaredDist(BasePoint, NearNeighbor->GetPosition());

		if (SquaredDist > 400.0f || bForceNearestNeighborSearch)
		{
			float Dist = std::sqrt(SquaredDist);
			Root->GetNearestNeighbor(BasePoint, NearNeighbor, Dist, SquaredDist);
		}

		ResultActor = NearNeighbor;
	}

	return ResultActor;
}
