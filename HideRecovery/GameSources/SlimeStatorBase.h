
/*!
@file SlimeStatorBase.h
@brief SlimeStatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//------------------------------------------------------------------------------------------------
		/// スライムのステートタイプ
		//------------------------------------------------------------------------------------------------
		enum class SlimeStateType {
			None,            //何もしない状態
			Start,           //開始イベント時の状態
			Plowling,        //徘徊
			Find,            //発見
			Chase,           //追従
			Evade,           //逃げる
			KnockBack,       //ノックバック
			Attack,          //攻撃
			Absorbed,        //吸収
			AbsorbedAttack,  //吸収状態の攻撃
			Pressed,         //潰れる状態
			Dyning,          //死亡演出状態
			Death,           //完全に死亡
		};

		//------------------------------------------------------------------------------------------------
		/// スライムステーターの規定クラス
		//------------------------------------------------------------------------------------------------
		template<class NodeType, class TransitionMember>
		class SlimeStatorBase : public StatorBase<NodeType, SlimeStateType, TransitionMember>
		{
		public:
			using State = SlimeStateType;

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SlimeStatorBase(const std::shared_ptr<GameObject>& objPtr)
				: SlimeStatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移条件用メンバー</param>
			SlimeStatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: StatorBase(objPtr, member)
			{}

		};
	}
}