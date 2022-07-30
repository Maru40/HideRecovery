
/*!
@file Stator_ShootSlime.h
@brief Stator_ShootSlimeなど
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

		namespace StateNode {
			struct Slime_Dyning_Parametor;
		}

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライムのステーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Stator_ShootSlime_TransitionMember {
				Trigger chaseTrigger;      //追従に切り替わるトリガー
				Trigger knockBackTrigger;  //ノックバックトリガー

				float startChaseRange;     //追従を始める距離
				float maxChaseRange;       //追跡する最大距離
				float attackStartRange;    //攻撃開始距離

				Stator_ShootSlime_TransitionMember();
				Stator_ShootSlime_TransitionMember(
					const float& startChaseRange,
					const float& maxChaseRange,
					const float& attackStartRange
				);
			};

			//--------------------------------------------------------------------------------------
			/// ショットスライムのステーターのブラックボード
			//--------------------------------------------------------------------------------------
			struct Stator_ShootSlime_BlackBoard
			{
				std::shared_ptr<Enemy::StateNode::Slime_Dyning_Parametor> deathParamPtr;  //死亡パラメータ
			};

			//--------------------------------------------------------------------------------------
			/// ショットスライムのステーター
			//--------------------------------------------------------------------------------------
			class Stator_ShootSlime : public SlimeStatorBase<EnemyBase, Stator_ShootSlime_TransitionMember>, public I_BrackBoard<Stator_ShootSlime_BlackBoard>
			{
			public:
				using TransitionMember = Stator_ShootSlime_TransitionMember;
				using BlackBoard = Stator_ShootSlime_BlackBoard;

				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr;  //パラメータ

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				/// <param name="member">遷移条件用メンバー</param>
				Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				/// <summary>
				/// パラメータの設定
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// 徘徊行動パラメータの設定
				/// </summary>
				void SettingPlowlingParametor();

				/// <summary>
				/// 攻撃パラメータの設定
				/// </summary>
				void SettingAttackParametor();

			};
		}
	}
}