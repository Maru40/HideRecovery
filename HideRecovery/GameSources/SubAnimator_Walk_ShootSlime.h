
/*!
@file SubAnimator_Walk_ShootSlime.h
@brief SubAnimator_Walk_ShootSlimeなど
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

		namespace ShootSlime {

			namespace SubAnimator {

				//--------------------------------------------------------------------------------------
				/// 移動サブアニメータ―のステートタイプ
				//--------------------------------------------------------------------------------------
				enum class Walk_StateType
				{
					Idle,        //待機
					Preliminary, //予備動作
					Jump,        //ジャンプ
					Smooth,      //伸びる
					Wait,        //硬直
				};

				//--------------------------------------------------------------------------------------
				/// 移動サブアニメーターの遷移条件メンバー
				//--------------------------------------------------------------------------------------
				struct Walk_TransitionMember
				{

				};

				//--------------------------------------------------------------------------------------
				/// 移動サブアニメータ―
				//--------------------------------------------------------------------------------------
				class Walk : public SubAnimatorBase<EnemyBase, Walk_StateType, Walk_TransitionMember>
				{
				public:
					using StateType = Walk_StateType;
					using TransitionMember = Walk_TransitionMember;

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
					Walk(const std::shared_ptr<GameObject>& objPtr);

					void CreateNode() override;
					void CreateEdge() override;

				};

			}

		}
	}
}