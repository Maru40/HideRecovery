
/*!
@file Utility_EnemyStator.h
@brief Utility_EnemyStatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;
	}

	class VelocityManager;

	namespace Enemy {

		namespace UtilityStator {

			//--------------------------------------------------------------------------------------
			/// 比較タイプ
			//--------------------------------------------------------------------------------------
			enum class ComparisonType
			{
				Less,     //未満
				OrLess,   //以下
				Greater,  //大きい
				OrGreater //以上
			};

			//--------------------------------------------------------------------------------------
			/// 遷移判断クラス
			//--------------------------------------------------------------------------------------
			class Transition
			{
			public:
				/// <summary>
				/// プレイヤーが視界に入った時の遷移判定
				/// </summary>
				/// <param name="selfObject">自分自身のオブジェクト</param>
				/// <param name="eyeRange">視界範囲</param>
				/// <returns>プレイヤーが視界に入ったらtrue</returns>
				static bool InEyeRangePlayer(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange);

				/// <summary>
				/// ターゲットが視界に入った時の遷移判定
				/// </summary>
				/// <param name="selfObject">自分自身のオブジェクト</param>
				/// <param name="eyeRange">視界範囲</param>
				/// <returns>ターゲットが視界に入った時にtrue</returns>
				static bool InEyeRangeTarget(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange);

				/// <summary>
				/// ターゲットをロストした遷移判定
				/// </summary>
				/// <param name="selfObject">自分自身のオブジェクト</param>
				/// <param name="maxChaseRange">追従最大距離</param>
				/// <returns>追従最大距離外ならtrue</returns>
				static bool LostTarget(const std::shared_ptr<GameObject>& selfObject, const float& maxChaseRange);

				/// <summary>
				/// 速度を比較して遷移を判断
				/// </summary>
				/// <param name="selfObject">自分自身のオブジェクト</param>
				/// <param name="transitionVelocity">遷移する速度</param>
				/// <param name="type">比較タイプ</param>
				/// <returns>比較タイプに合わせて条件に合致したらtrue</returns>
				static bool ComparisonVelocity(const std::shared_ptr<GameObject>& selfObject,
					const float& transitionVelocity,
					const ComparisonType& type);

				/// <summary>
				/// 自分自身が終了した時(EnemyMainStateMachineではまだ非対応)
				/// </summary>
				/// <param name="stateMachine">ステートマシン</param>
				/// <param name="state">終了を判断したいステート</param>
				/// <returns>ステートが終了していたらtrue</returns>
				template<class node_type, class StateEnum, class TransitionMember>
				static bool IsSelfSetateEnd(
					const std::unique_ptr<maru::AnimationMainStateMachine<node_type, StateEnum, TransitionMember>>& stateMachine,
					const StateEnum& state)
				{
					//ステートが自分自身
					if (stateMachine->IsCurrentState(state)) {
						return stateMachine->GetNowNode()->GetAnimationClip()->IsEnd();  //ステートが終了したら。
					}

					return false; 
				}

			};
		}
	}
}