
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrolなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StateMachine/Node_StateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				//class HidePlacePatrolTree;
				class ButtleTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class FindTarget;
			class Damaged;
			class Kill;
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	バトルステート
			//--------------------------------------------------------------------------------------
			class Buttle : public maru::NodeBase_StateMachine<EnemyBase>
			{
			public:
				using ButtleTree = maru::Behavior::SubBehavior::ButtleTree;

			private:
				std::weak_ptr<Transform> m_transform;
				std::weak_ptr<TargetManager> m_targetManager;

				std::unique_ptr<ButtleTree> m_behaviorTree;

			public:
				Buttle(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;

			private:

				/// <summary>
				/// 通知設定
				/// </summary>
				void SettingNotify();

				/// <summary>
				/// ターゲット発見メッセージを監視する。
				/// </summary>
				/// <param name="tuple"></param>
				void ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

				/// <summary>
				/// ダメージメッセージを監視する。
				/// </summary>
				/// <param name="tuple">ダメージメッセージ</param>
				void ObserveDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// 自分自身へのダメージ処理
				/// </summary>
				/// <param name="tuple">ダメージメッセージ</param>
				void SelfDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// 自分以外がダメージを受けた時
				/// </summary>
				/// <param name="tuple">ダメージメッセージ</param>
				void OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// キルメッセージを受け取ったとき。
				/// </summary>
				/// <param name="tuple"></param>
				void NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple);

				bool HasTarget() const;

			};

		}
	}
}