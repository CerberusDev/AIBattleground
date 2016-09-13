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

bool QuadTree::NoActorsInTree() const
{
	return (Root->TopRightChild == nullptr && Root->Actors.size() == 0);
}

Actor* QuadTree::FindNearestNeighborTo(sf::Vector2f BasePoint)
{
	if (NoActorsInTree())
		return nullptr;

	Actor* ResultActor = Root->GetNaiveNeighbor(BasePoint);
	float SquaredDist = GetSquaredDist(BasePoint, ResultActor->GetPosition());
	float Dist = std::sqrt(SquaredDist);
	Root->GetNearestNeighbor(BasePoint, ResultActor, Dist, SquaredDist);

	return ResultActor;
}

Actor* QuadTree::QuickFindNearNeighborTo(Actor* RequestingActor)
{
	if (NoActorsInTree())
		return nullptr;

	sf::Vector2f LastEnemyPosition = RequestingActor->GetNearestEnemy()->GetPosition();
	Actor* ResultActor = Root->GetNaiveNeighbor(LastEnemyPosition);

	if (GetSquaredDist(LastEnemyPosition, ResultActor->GetPosition()) > MAX_ACCEPTABLE_SQUARED_DIST_DURING_QUICK_SEARCH)
		ResultActor = FindNearestNeighborTo(RequestingActor->GetPosition());

	return ResultActor;
}
