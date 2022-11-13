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

			namespace SubBehavior {

				enum class HidePlacePatrolTree_BehaviorType {
					FirstSelecter,		//初回ノード

					ToBallRunTask,		//ボールまで駆けつけるタスク。
					PatrolTask,			//パトロールタスク
				};

				//--------------------------------------------------------------------------------------
				/// 隠れ場所を探すパトロールビヘイビアツリー
				//--------------------------------------------------------------------------------------
				class HidePlacePatrolTree : public SubBehaviorTreeBase<HidePlacePatrolTree_BehaviorType>
				{
				public:
					using BehaviorType = HidePlacePatrolTree_BehaviorType;

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