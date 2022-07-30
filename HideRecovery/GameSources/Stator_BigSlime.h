
/*!
@file Stator_BigSlime.h
@brief Stator_BigSlimeなど
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
			struct BigSlime_Dyning_Parametor;
		}

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// 巨大スライムステーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Stator_TransitionMemeber {
				Trigger chaseTrigger;      //追従に切り替わるトリガー
				Trigger knockBackTrigger;  //ノックバックトリガー
				float startChaseRange;     //追従を始める距離
				float maxChaseRange;       //追跡する最大距離
				float bodyBlowRange;       //体当たりを始める距離

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Stator_TransitionMemeber();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="startChaseRange">追従を始める距離</param>
				/// <param name="maxChaseRange">追従をする最大距離</param>
				/// <param name="bodyBlowRange">体当たりを始める距離</param>
				Stator_TransitionMemeber(const float& startChaseRange, const float& maxChaseRange, const float& bodyBlowRange);
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムステーターのブラックボード
			//--------------------------------------------------------------------------------------
			struct Stator_BlackBoard {
				std::shared_ptr<StateNode::Slime_Dyning_Parametor> dyningParamPtr;  //死亡
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムステーター
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
				/// <param name="member">遷移条件メンバー</param>
				Stator(const std::shared_ptr<GameObject>& owner, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				//--------------------------------------------------------------------------------------
				/// 遷移条件
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// ボディブローをする距離
				/// </summary>
				/// <param name="member">遷移条件メンバー</param>
				/// <returns>ボディブローをする距離ならtrue</returns>
				bool Stator::IsBodyBlowRange(const Stator::TransitionMember& member);

				//--------------------------------------------------------------------------------------
				/// パラメータ設定
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// パラメータの設定
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// 徘徊パラメータの設定
				/// </summary>
				void SettingPlowling();

				/// <summary>
				/// 追従パラメータの設定
				/// </summary>
				void SettingChase();

			};

		}
	}
}