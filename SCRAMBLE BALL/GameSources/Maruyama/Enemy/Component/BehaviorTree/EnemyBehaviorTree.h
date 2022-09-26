/*!
@file EnemyBehaviorTree.h
@brief EnemyBehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTreeComponentBase.h"

namespace basecross {

	namespace Enemy {

		enum class EnemyBehaviorTreeType {
			FirstSelecter,
			ChaseSelecter,
			AttackSelecter,

			ChaseTask,
		};

		class EnemyBehaviorTree : public BehaviorTreeComponentBase<EnemyBehaviorTreeType>
		{
		public:
			using BehaviorType = EnemyBehaviorTreeType;

		private:

		public:
			EnemyBehaviorTree(const std::shared_ptr<GameObject>& objPtr);

		private:
			void CreateNode() override;
			void CreateEdge() override;
		};

	}
}