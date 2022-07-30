
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
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace StateNode {
			struct Slime_Dyning_Parametor;
		}

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムステーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Stator_EvadeSlime_TransitionMember {
				Trigger knockBackTrigger;  //ノックバックトリガー

				float startEvadeRange;     //逃げると判断する距離
				float endEvadeRange;       //逃げ切ったと判断する距離
				
				/// <summary>
				/// コンストラクタ
				/// </summary>
				Stator_EvadeSlime_TransitionMember();

				/// <summary>
				/// 
				/// </summary>
				/// <param name="startEvadeRange"></param>
				/// <param name="endEvadeRange"></param>
				Stator_EvadeSlime_TransitionMember(const float& startEvadeRange, const float& endEvadeRange);
			};

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムステーターのブラックボード
			//--------------------------------------------------------------------------------------
			struct Stator_EvadeSlime_BlackBoard
			{
				std::shared_ptr<Enemy::StateNode::Slime_Dyning_Parametor> deathParamPtr;  //死亡
			};

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムのステーター
			//--------------------------------------------------------------------------------------
			class Stator_EvadeSlime : public SlimeStatorBase<EnemyBase, Stator_EvadeSlime_TransitionMember>, public I_BrackBoard<Stator_EvadeSlime_BlackBoard>
			{
			public:
				using TransitionMember = Stator_EvadeSlime_TransitionMember;

				/// <summary>
				/// パラメータ
				/// </summary>
				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr = nullptr;  //パラメータ

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				/// <param name="member">遷移条件メンバー</param>
				Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				/// <summary>
				/// パラメータの設定
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// 徘徊パラメータの設定
				/// </summary>
				void SettingPlowlingParametor();

			};

		}
	}
}