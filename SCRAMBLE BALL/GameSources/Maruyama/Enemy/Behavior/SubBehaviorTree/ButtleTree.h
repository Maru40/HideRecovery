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

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリー
				//--------------------------------------------------------------------------------------
				enum class ButtleTree_BihaviorType {
					FirstSelecter,		//初期セレクター

					AttackSelecter,		//攻撃(シーケンス(移動が必要なければ飛ばす))
						AttackMoveSelecter,	//攻撃中の移動セレクター
							NearSeekMoveTask,	//近づくタスク(気づかれてなかったら、距離詰めたいかも)
							WanparoundSelecter,	//回り込む(見方が打ち合っていたら)
								RightSideMoveTask,	//右サイド
								LeftSideMoveTask,	//左サイド 
								BackSideMoveTask,	//裏取り
						ShotTask,			//撃つ

					EvadeSelecter,	//退避セレクター
						InducementStaySelecter,	//足止めセレクター
							InducementAttack,	//足止め攻撃
							//逃げる(自分をおとりに、攻撃させたくなる動き)
						EvadeMoveTask,	//退避移動
				};

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリー
				//--------------------------------------------------------------------------------------
				class ButtleTree : public SubBehaviorTreeBase<ButtleTree_BihaviorType>
				{
				public:
					using NodeType = ButtleTree_BihaviorType;

				private:


				public:
					ButtleTree(const std::shared_ptr<GameObject>& owner);

				protected:
					void CreateNode() override;

					void CreateEdge() override;

					void CreateDecorator() override;

				};

			}
		}
	}
}