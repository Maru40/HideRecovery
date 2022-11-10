
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrolなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class ObserveIsInEyeTarget;
	class I_TeamMember;
	class EyeSearchRange;
	class TargetManager;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				class HidePlacePatrolTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class Damaged;
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	隠し場所を探すステート
			//--------------------------------------------------------------------------------------
			class HidePlacePatrol : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using HidePlacePatrolTree = maru::Behavior::SubBehavior::HidePlacePatrolTree;

			private:
				std::unique_ptr<HidePlacePatrolTree> m_behaviorTree;			//ビヘイビアツリー

				std::unique_ptr<ObserveIsInEyeTarget> m_observeButtleTarget;	//バトル用に監視したい対象

				std::weak_ptr<I_TeamMember> m_teamMember;			//チームメンバー
				std::weak_ptr<I_FactionMember> m_factionMember;		//ファクションメンバー
				std::weak_ptr<TargetManager> m_targetManager;

			public:
				HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner);

				virtual ~HidePlacePatrol() = default;

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:

				/// <summary>
				/// ダメージを受けた時の処理
				/// </summary>
				void Damaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// バトル用のターゲットを監視する。
				/// </summary>
				void ObserveButtleTarget();

				/// <summary>
				/// 隠し場所を探しているときのターゲットを設定する。
				/// </summary>
				void SettingObserveButtleTargets();
				
			};

		}
	}
}