/*!
@file EnemyStatorBase.h
@brief EnemyStatorBaseなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	ステートタイプ
		//--------------------------------------------------------------------------------------
		enum class StateType
		{
			None,            //何もしない状態
			Start,           //開始イベント時の状態
			Plowling,        //徘徊
			Find,            //発見
			Chase,           //追従
			Evade,           //逃げる
			KnockBack,       //ノックバック
			Attack,          //攻撃
			Pressed,         //潰れる状態
			Dyning,          //死亡演出状態
			Death,           //完全に死亡
		};

		//--------------------------------------------------------------------------------------
		///	エネミーステーターの基底クラス
		//--------------------------------------------------------------------------------------
		template<class NodeType, class TransitionMember>
		class EnemyStatorBase : public StatorBase<NodeType, StateType, TransitionMember>
		{
		public:
			using State = StateType;

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			EnemyStatorBase(const std::shared_ptr<GameObject>& objPtr)
				: SlimeStatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移条件用メンバー</param>
			EnemyStatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: StatorBase(objPtr, member)
			{}
		};

	}
}