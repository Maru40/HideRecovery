/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/SubBehaviorTree/SubBehaviorTreeBase.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace Task {
				struct NearAstarMove_Parametor;
				struct NearSeekMove_Parametor;
			}

			namespace SubBehavior {

				enum class HidePlacePatrolTree_BehaviorType {
					FirstSelecter,			//初回セレクター

					ToGoalRunTask,			//ゴールまで行くタスク。
					ToMoveHasBallEnemyTask,	//ボールを持っている敵まで行くタスク。
					RelifHasBallMemberTask,	//ボールを持っているメンバーを守るタスク。
					ToBallRunTask,			//ボールまで駆けつけるタスク。
					PatrolTask,				//パトロールタスク。
				};

				//--------------------------------------------------------------------------------------
				/// 隠れ場所を探すパトロールビヘイビアツリーのパラメータ
				//--------------------------------------------------------------------------------------
				struct HidePlacePatrolTree_Parametor {

					HidePlacePatrolTree_Parametor();

					~HidePlacePatrolTree_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// 隠れ場所を探すパトロールビヘイビアツリー
				//--------------------------------------------------------------------------------------
				class HidePlacePatrolTree : public SubBehaviorTreeBase<HidePlacePatrolTree_BehaviorType>
				{
				public:
					using Parametor = HidePlacePatrolTree_Parametor;
					using BehaviorType = HidePlacePatrolTree_BehaviorType;

				private:
					Parametor m_param;

				public:
					HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr);

					virtual ~HidePlacePatrolTree() = default;

				protected:

					void CreateNode() override;
					void CreateEdge() override;
					void CreateDecorator() override;

				public:

				};

			}
		}
	}
}