/*!
@file CollisionAction.h
@brief CollisionAction
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	衝突イベント
		//--------------------------------------------------------------------------------------
		class CollisionAction : public Component
		{
			/// <summary>
			/// 衝突イベントの「開始」、「更新」、「終了」をまとめたもの
			/// </summary>
			template<class T>
			struct ActionTrio {
				maru::Action<T> enter;   //衝突開始イベント
				maru::Action<T> excute;  //衝突更新イベント
				maru::Action<T> exit;    //衝突終了イベント
			};

			ActionTrio<void(const CollisionPair&)> m_collisionAction;  //コリジョンアクション
			ActionTrio<void(const CollisionPair&)> m_triggerAction;    //トリガーアクション

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			CollisionAction(const std::shared_ptr<GameObject>& objPtr);

			//--------------------------------------------------------------------------------------
			///	衝突判定
			//--------------------------------------------------------------------------------------

			void OnCollisionEnter(const CollisionPair& pair) override;
			void OnCollisionExcute(const CollisionPair& pair) override;
			void OnCollisionExit(const CollisionPair& pair) override;

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------


			//--------------------------------------------------------------------------------------
			///	Collision
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 衝突開始イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddCollisionEnter(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.enter.AddFunction(function);
			}

			/// <summary>
			/// 衝突更新イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddCollisionExcute(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.excute.AddFunction(function);
			}

			/// <summary>
			/// 衝突終了イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddCollisionExit(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.exit.AddFunction(function);
			}


			//--------------------------------------------------------------------------------------
			///	Trigger
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// Trigger開始イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddTriggerEnter(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.enter.AddFunction(function);
			}

			/// <summary>
			/// Trigger更新イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddTriggerExcute(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.excute.AddFunction(function);
			}

			/// <summary>
			/// Trigger終了イベントの追加
			/// </summary>
			/// <param name="function">イベント</param>
			void AddTriggerExit(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.exit.AddFunction(function);
			}
		};
	}

}