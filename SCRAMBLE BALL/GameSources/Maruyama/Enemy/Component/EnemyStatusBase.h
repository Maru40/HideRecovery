
/*!
@file EnemyStatusBase.h
@brief EnemyStatusBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	エネミーのステータス
		//--------------------------------------------------------------------------------------
		struct EnemyStatus
		{
			float hp;                    //ヒットポイント
			float maxHp;                 //最大ヒットポイント
			float toughness;             //強靭度
			float baseToughness;         //初期強靭度

			EnemyStatus();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="maxHp">最大HP</param>
			/// <param name="toughness">強靭度</param>
			EnemyStatus(const float& maxHp, const float& toughness);
		};

		//--------------------------------------------------------------------------------------
		///	エネミーのステータス管理の基底クラス
		//--------------------------------------------------------------------------------------
		class EnemyStatusManagerBase : public Component
		{
		protected:
			EnemyStatus m_status;  //ステータス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">自分自身を所有するゲームオブジェクト</param>
			EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">自分自身を所有するゲームオブジェクト</param>
			/// <param name="status">ステータス</param>
			EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status);

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ステータスのセット
			/// </summary>
			/// <param name="status">ステータス</param>
			void SetStatus(const EnemyStatus& status) noexcept;

			/// <summary>
			/// ステータスの取得
			/// </summary>
			/// <returns>ステータス</returns>
			EnemyStatus GetStatus() const noexcept;

			/// <summary>
			/// ステータスの参照を取得
			/// </summary>
			/// <returns>ステータスの参照</returns>
			EnemyStatus& GetRefStatus();

			/// <summary>
			/// 強靭度を超える力かどうか
			/// </summary>
			/// <param name="power">与えられるパワー</param>
			/// <returns>強靭度を超えたらtrue</returns>
			bool IsToughnessBreak(const float& power);
		};

	}
}