
/*!
@file AIPlayerStator.h
@brief AIPlayerStatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StatorBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class ObserveIsInEyeTarget;
	class I_TeamMember;
	class TargetManager;

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class I_Tupler;
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorのステートタイプ
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//隠し場所探す。
			Buttle,				//バトル
			Goal,				//ゴール中
			Dyning,				//死亡中
			Dead,				//死亡
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {
			float hidePatrolEeyRange;	//隠し場所を探しているときの視界範囲
			float buttleStartEyeRange;	//バトル開始時の視界範囲

			AIPlayerStator_TransitionMember();
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
			std::weak_ptr<Tuple::I_Tupler> m_tupler;		//タプルスペースを使う者
			std::weak_ptr<TargetManager> m_targetManager;	//ターゲット管理
			
		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			virtual ~AIPlayerStator() = default;

			void OnLateStart() override;

			void CreateNode() override;
			void CreateEdge() override;

			void OnEnable() override;

		private:

			//--------------------------------------------------------------------------------------
			///	遷移条件系
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// バトルターゲット発見
			/// </summary>
			/// <param name="member">遷移条件メンバー</param>
			/// <returns>バトルターゲット</returns>
			bool IsFindButtleTarget(const TransitionMember& member);

			/// <summary>
			/// ターゲットを見失ったときの遷移
			/// </summary>
			/// <param name="member">遷移条件メンバー</param>
			/// <returns>ターゲットを見失ったらtrue</returns>
			bool IsLostButtleTarget(const TransitionMember& member);

		};

	}
}