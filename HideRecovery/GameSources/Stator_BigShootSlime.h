
/*!
@file Stator_EvadeSlime.h
@brief Stator_EvadeSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace StateNode {
			struct Slime_Dyning_Parametor;
			struct BigSlime_Dyning_Parametor;
		}

		namespace BigShootSlime {

			//--------------------------------------------------------------------------------------
			///	ステートの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Stator_TransitionMemeber {
				Trigger chaseTrigger;      //追従に切り替わるトリガー
				Trigger knockBackTrigger;  //ノックバックトリガー
				float startChaseRange;     //追従を始める距離
				float maxChaseRange;       //追跡する最大距離
				float bodyBlowRange;       //体当たりを始める距離

				Stator_TransitionMemeber();
				Stator_TransitionMemeber(const float& startChaseRange, const float& maxChaseRange, const float& bodyBlowRange);
			};

			//--------------------------------------------------------------------------------------
			///	ステートのブラックボード
			//--------------------------------------------------------------------------------------
			struct Stator_BlackBoard {
				std::shared_ptr<StateNode::Slime_Dyning_Parametor> dyningParamPtr;  //死亡
			};

			//--------------------------------------------------------------------------------------
			///	巨大ショットスライムのステーター
			//--------------------------------------------------------------------------------------
			class Stator : public SlimeStatorBase<EnemyBase, Stator_TransitionMemeber>, public I_BrackBoard<Stator_BlackBoard>
			{
			public:
				using TransitionMember = Stator_TransitionMemeber;
				using BlackBoard = Stator_BlackBoard;

				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr = nullptr; //パラメータ

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				Stator(const std::shared_ptr<GameObject>& owner);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				/// <param name="member">遷移条件用メンバー</param>
				Stator(const std::shared_ptr<GameObject>& owner, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				//--------------------------------------------------------------------------------------
				///	遷移条件
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// ボディブローする距離かどうか
				/// </summary>
				/// <param name="member">遷移条件用メンバー</param>
				/// <returns>ボディブローする距離ならtrue</returns>
				bool Stator::IsBodyBlowRange(const Stator::TransitionMember& member);

				//--------------------------------------------------------------------------------------
				///	パラメータ設定
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// パラメータの設定
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// 俳諧行動パラメータの設定
				/// </summary>
				void SettingPlowling();

				/// <summary>
				/// 追従行動パラメータの設定
				/// </summary>
				void SettingChase();

			};

		}
	}
}