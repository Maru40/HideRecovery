/*!
@file EnemyBehaviorTree.h
@brief EnemyBehaviorTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTreeComponentBase.h"

namespace basecross {

	namespace Enemy {

		enum class EnemyBehaviorTreeType {
			FirstSelecter,	//初期セレクター

			ButtleSelecter,	//バトルセレクター
				AttackSelecter,	//攻撃セレクター


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