// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <vector>

#include "AISystemBase.h"
#include "Actor.h"

class AISystemBT : public AISystemBase
{
private:
	struct BTNode
	{
		virtual void Update() = 0;
	};

	struct BTComposite : public BTNode
	{
		std::vector<BTNode*> Childs;

		virtual void Update()
		{
			for (auto it = Childs.begin(); it != Childs.end(); ++it)
				(*it)->Update();
		}

		void AddChild(BTNode* NewChild)
		{
			Childs.push_back(NewChild);
		}

		virtual ~BTComposite()
		{
			for (auto it = Childs.begin(); it != Childs.end(); ++it)
				delete *it;
		}
	};

	struct BTTask : public BTNode
	{
		Actor* OwningActor;
		BTTask(Actor* argOwningActor) : OwningActor(argOwningActor) {};
	};

	struct BTTask_StopMovement : public BTTask
	{
		BTTask_StopMovement(Actor* argOwningActor) : BTTask(argOwningActor) {};

		virtual void Update()
		{
			OwningActor->StopMovement();
		}
	};

	struct BTTask_GoTowardsNearestEnemy : public BTTask
	{
		BTTask_GoTowardsNearestEnemy(Actor* argOwningActor) : BTTask(argOwningActor) {};

		virtual void Update()
		{
			OwningActor->GoTowardsNearestEnemy();
		}
	};

	BTNode* Root;

public:
	AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemBT();

	void Update();
};
