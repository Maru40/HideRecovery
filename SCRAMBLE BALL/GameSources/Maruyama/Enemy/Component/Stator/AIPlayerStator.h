
/*!
@file AIPlayerStator.h
@brief AIPlayerStatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StatorBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class ObserveIsInEyeTarget;
	class I_TeamMember;

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorのステートタイプ
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//隠し場所探す。
			Buttle,				//バトル
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {
			float hidePatrolEeyRange = 5.0f;	//隠し場所を探しているときの視界範囲

			AIPlayerStator_TransitionMember() = default;
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator
		//--------------------------------------------------------------------------------------
		class AIPlayerStator : public StatorBase<EnemyBase, AIPlayerStator_StateType, AIPlayerStator_TransitionMember>
		{
		public:
			using StateType = AIPlayerStator_StateType;
			using TransitionMember = AIPlayerStator_TransitionMember;

		private:
			std::weak_ptr<EyeSearchRange> m_eye;			//視界管理
			std::weak_ptr<I_TeamMember> m_teamMember;		//チームメンバーインターフェース
			std::weak_ptr<I_FactionMember> m_factionMember;	//ファクションメンバーインターフェース

			std::unique_ptr<ObserveIsInEyeTarget> m_observeButtleTarget;	//バトル用に監視したい対象

		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			virtual ~AIPlayerStator() = default;

			void OnLateStart() override;

			void CreateNode() override;
			void CreateEdge() override;

		private:

			/// <summary>
			/// バトル対象として監視するターゲットをセッティング
			/// </summary>
			void SettingObserveButtleTargets();

			//--------------------------------------------------------------------------------------
			///	遷移条件系
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// バトルターゲット発見
			/// </summary>
			/// <param name="member">遷移条件メンバー</param>
			/// <returns>バトルターゲット</returns>
			bool IsFindButtleTarget(const TransitionMember& member);

		};

	}
}