/*!
@file SubAnimator_WalkJump.h
@brief SubAnimator_WalkJumpなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace SubAnimator {

			//--------------------------------------------------------------------------------------
			/// ジャンプ移動サブアニメーターのステートタイプ
			//--------------------------------------------------------------------------------------
			enum class WalkJump_StateType
			{
				Idle,        //待機
				Preliminary, //予備動作
				Jump,        //ジャンプ
				Wait,        //硬直
			};

			//--------------------------------------------------------------------------------------
			/// ジャンプ移動サブアニメーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct WalkJump_TransitionMember
			{

			};

			//--------------------------------------------------------------------------------------
			///ジャンプ移動サブアニメーター
			//--------------------------------------------------------------------------------------
			class WalkJump : public SubAnimatorBase<EnemyBase, WalkJump_StateType, WalkJump_TransitionMember>
			{
			public:
				using StateType = WalkJump_StateType;
				using TransitionMember = WalkJump_TransitionMember;

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				WalkJump(const std::shared_ptr<GameObject>& objPtr);

				void CreateNode() override;
				void CreateEdge() override;

			};

		}
	}
}