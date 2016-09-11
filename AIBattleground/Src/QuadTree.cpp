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
