/*!
@file AIPlayerBehaviorTree.h
@brief AIPlayerBehaviorTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTreeComponentBase.h"

namespace basecross {

	namespace Enemy {

		enum class EnemyBehaviorTreeType {
			FirstSelecter,		//初期セレクター

			PlowlingSelecter,	//徘徊セレクター
				SearchBallTask,		//ボールを探す

			ButtleSelecter,		//バトルセレクター
				AttackSelecter,		//攻撃セレクター
		};

		class AIPlayerBehaviorTree : public BehaviorTreeComponentBase<EnemyBehaviorTreeType>
		{
		public:
			using BehaviorType = EnemyBehaviorTreeType;

		private:

		public:
			AIPlayerBehaviorTree(const std::shared_ptr<GameObject>& objPtr);

		private:
			void CreateNode() override;
			void CreateEdge() override;
		};

	}
}