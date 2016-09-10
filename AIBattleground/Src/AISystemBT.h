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
	bool BTStorage_Bool;

private:
	enum class EStatus { SUCCESS, FAIL, IN_PROGRESS };

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
				else if (ChildStatus == EStatus::IN_PROGRESS)
				{
					return EStatus::IN_PROGRESS;
				}
			}

			return EStatus::FAIL;
		}
	};

	struct BTSequence : public BTComposite
	{
		BTSequence(std::vector<BTNode*> argChildren) : BTComposite(argChildren) {};
		virtual EStatus Update()
		{
			for (auto it = Children.begin(); it != Children.end(); ++it)
			{
				EStatus ChildStatus = (*it)->Update();

				if (ChildStatus == EStatus::FAIL)
				{
					return EStatus::FAIL;
				}
				else if (ChildStatus == EStatus::IN_PROGRESS)
				{
					return EStatus::IN_PROGRESS;
				}
			}

			return EStatus::SUCCESS;
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

	struct BTDecorator_BTStorage_Bool : public BTDecorator
	{
		AISystemBT* AISystem;

		BTDecorator_BTStorage_Bool(AISystemBT* argAISystem, BTNode* argChild) : BTDecorator(argChild), AISystem(argAISystem) {};

		virtual EStatus Update()
		{
			if (AISystem->BTStorage_Bool)
				return Child->Update();
			else
				return EStatus::FAIL;
		}
	};

	struct BTBlackboardDecorator : public BTDecorator
	{
		class Blackboard* AIBlackboard;

		BTBlackboardDecorator(class Blackboard* argBlackboard, BTNode* argChild) : BTDecorator(argChild), AIBlackboard(argBlackboard) {};
	
		virtual EStatus Update()
		{
			if (IsConditionFulfilled())
				return Child->Update();
			else
				return EStatus::FAIL;
		}

		virtual bool IsConditionFulfilled() = 0;
	};

	struct BTBDecorator_EnemyInRange : public BTBlackboardDecorator
	{
		BTBDecorator_EnemyInRange(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};
	
		virtual bool IsConditionFulfilled() 
		{
			return AIBlackboard->GetBEnemyInRange();
		}
	};

	struct BTBDecorator_NearestEnemySet : public BTBlackboardDecorator
	{
		BTBDecorator_NearestEnemySet(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};

		virtual bool IsConditionFulfilled()
		{
			return AIBlackboard->GetNearestEnemy() != nullptr;
		}
	};

	struct BTBDecorator_LowHealth : public BTBlackboardDecorator
	{
		BTBDecorator_LowHealth(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};

		virtual bool IsConditionFulfilled()
		{
			return AIBlackboard->GetHP() < AIBlackboard->GetMaxHP() * 0.5f;
		}
	};

	struct BTBDecorator_FullHealth : public BTBlackboardDecorator
	{
		BTBDecorator_FullHealth(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};

		virtual bool IsConditionFulfilled()
		{
			return AIBlackboard->GetHP() == AIBlackboard->GetMaxHP();
		}
	};

	struct BTBDecorator_HealZoneReached : public BTBlackboardDecorator
	{
		BTBDecorator_HealZoneReached(class Blackboard* argBlackboard, BTNode* argChild) : BTBlackboardDecorator(argBlackboard, argChild) {};

		virtual bool IsConditionFulfilled()
		{
			return AIBlackboard->GetBHealthZoneDestReached();
		}
	};

	struct BTTask : public BTNode
	{
		Actor* OwningActor;

		BTTask(Actor* argOwningActor) : OwningActor(argOwningActor) {};
	};

	struct BTTask_SetBTStorage_Bool : public BTNode
	{
		AISystemBT* AISystem;
		bool ValueToSet;

		BTTask_SetBTStorage_Bool(AISystemBT* argAISystem, bool argValueToSet) : AISystem(argAISystem), ValueToSet(argValueToSet) {};

		virtual EStatus Update()
		{
			AISystem->BTStorage_Bool = ValueToSet;
			return EStatus::SUCCESS;
		}
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
			return EStatus::IN_PROGRESS;
		}
	};

	struct BTTask_Fight : public BTTask
	{
		BTTask_Fight(Actor* argOwningActor) : BTTask(argOwningActor) {};

		virtual EStatus Update()
		{
			OwningActor->TryToShoot();
			return EStatus::IN_PROGRESS;
		}
	};

	struct BTTask_RetreatToHealZone : public BTTask
	{
		BTTask_RetreatToHealZone(Actor* argOwningActor) : BTTask(argOwningActor) {};

		virtual EStatus Update()
		{
			OwningActor->RetreatToHealZone();
			return EStatus::IN_PROGRESS;
		}
	};

	BTNode* Root;

public:
	AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemBT();

	void Update();
};
