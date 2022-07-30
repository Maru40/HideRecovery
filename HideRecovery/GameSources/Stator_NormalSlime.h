
/*!
@file Stator_NormalSlime.h
@brief Stator_NormalSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		template<class NodeType, class EnumType, class TransitionMember>
		class StatorBase;

		namespace StateNode {
			struct Slime_Dyning_Parametor;
			struct Slime_Attack_Parametor;
		}

		//--------------------------------------------------------------------------------------
		///	通常スライムのステーターの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct Stator_NormalSlime_TransitionMember
		{
			Trigger chaseTrigger;      //追従に切り替わるトリガー
			Trigger knockBackTrigger;  //ノックバックトリガー
			float startChaseRange;     //追従を始める距離
			float maxChaseRange;       //追跡する最大距離
			float bodyBlowRange;       //体当たりを始める距離

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Stator_NormalSlime_TransitionMember();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="startChaseRange">追従を始める距離</param>
			/// <param name="maxChaseRange">追従する最大距離</param>
			/// <param name="bodyBlowRange">体当たりを始める距離</param>
			Stator_NormalSlime_TransitionMember(const float& startChaseRange, const float& maxChaseRange, const float& bodyBlowRange);
		};

		//--------------------------------------------------------------------------------------
		/// 通常スライムのステーターのブラックボード
		//--------------------------------------------------------------------------------------
		struct Stator_NormalSlime_BlackBoard
		{
			std::shared_ptr<StateNode::Slime_Dyning_Parametor> deathParamPtr;  //死亡
			std::shared_ptr<StateNode::Slime_Attack_Parametor> attackParamPtr; //攻撃
		};

		//--------------------------------------------------------------------------------------
		/// 通常スライムのステーター
		//--------------------------------------------------------------------------------------
		class Stator_NormalSlime : public SlimeStatorBase<EnemyBase, Stator_NormalSlime_TransitionMember>, public I_BrackBoard<Stator_NormalSlime_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_NormalSlime_TransitionMember;
			using BlackBoard = Stator_NormalSlime_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移条件メンバー</param>
			Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

		private:
			void CreateNode() override;
			void CreateEdge() override;

			/// <summary>
			/// ボディブローに遷移する距離かどうか
			/// </summary>
			/// <param name="member">遷移メンバー</param>
			/// <returns>遷移できる距離ならtrue</returns>
			bool IsBodyBlowRange(const Stator_NormalSlime::TransitionMember& member);

			/// <summary>
			/// パラメータの設定
			/// </summary>
			void SettingParametor();

			/// <summary>
			/// 徘徊パラメータ設定
			/// </summary>
			void SettingPlowlingParametor();

			/// <summary>
			/// 追従パラメータ設定
			/// </summary>
			void SettingChaseParametor();

			/// <summary>
			/// 攻撃パラメータ設定
			/// </summary>
			void SettingAttackParametor();

		};

	}
}