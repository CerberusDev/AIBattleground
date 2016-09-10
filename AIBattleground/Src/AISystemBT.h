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
	enum class EStatus { SUCCESS, FAIL };

	struct BTNode
	{
		virtual EStatus Update() = 0;
	};

	struct BTComposite : public BTNode
	{
		std::vector<BTNode*> Children;

		BTComposite(std::vector<BTNode*> argChildren) : Children(argChildren) {};
		virtual ~BTComposite()
		{
			for (auto it = Children.begin(); it != Children.end(); ++it)
				delete *it;
		}
	};

	struct BTSelector : public BTComposite
	{
		BTSelector(std::vector<BTNode*> argChildren) : BTComposite(argChildren) {};
		virtual EStatus Update()
		{
			for (auto it = Children.begin(); it != Children.end(); ++it)
			{
				EStatus ChildStatus = (*it)->Update();

				if (ChildStatus == EStatus::SUCCESS)
				{
					return EStatus::SUCCESS;
				}
			}

			return EStatus::FAIL;
		}
	};

	struct BTDecorator : public BTNode
	{
		BTNode* Child;

		BTDecorator(BTNode* argChild) : Child(argChild) {};
		virtual ~BTDecorator()
		{
			delete Child;
		}
	};

	struct BTBlackboardDecorator : public BTDecorator
	{
		class Blackboard* AIBlackboard;

		BTBlackboardDecorator(class Blackboard* argBlackboard, BTNode* argChild) : BTDecorator(argChild), AIBlackboard(argBlackboard) {};
	};

	struct BTBDecorator_EnemyInRange : public BTBlackboardDecorator
	{
		BTBDecorator_EnemyInRange(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};
	
		virtual EStatus Update()
		{
			return AIBlackboard->GetBEnemyInRange() ? EStatus::SUCCESS : EStatus::FAIL;
		}
	};

	struct BTBDecorator_NearestEnemySet : public BTBlackboardDecorator
	{
		BTBDecorator_NearestEnemySet(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};

		virtual EStatus Update()
		{
			if (AIBlackboard->GetNearestEnemy())
				return Child->Update();
			else
				return EStatus::FAIL;
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

		virtual EStatus Update()
		{
			OwningActor->StopMovement();
			return EStatus::SUCCESS;
		}
	};

	struct BTTask_GoTowardsNearestEnemy : public BTTask
	{
		BTTask_GoTowardsNearestEnemy(Actor* argOwningActor) : BTTask(argOwningActor) {};

		virtual EStatus Update()
		{
			OwningActor->GoTowardsNearestEnemy();
			return EStatus::SUCCESS;
		}
	};

	BTNode* Root;

public:
	AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemBT();

	void Update();
};
