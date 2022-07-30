/*!
@file Task_BodyBlow.h
@brief Task_BodyBlowなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	struct DamageData;

	namespace Enemy {
		class EnemyBase;

		//--------------------------------------------------------------------------------------
		/// ボディーブロータスクのパラメータ
		//--------------------------------------------------------------------------------------
		struct Task_BodyBlow_Parametor
		{
			float speed;                        //移動スピード
			float range;                        //移動距離
			float endVelocitySpeed;             //タックル後にどの減速スピードで終了するか
			float elapsedRange = 0.0f;          //計測距離
			unique_ptr<DamageData> damageData;  //ダメージデータ
			float toughness;                    //強靭度

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Task_BodyBlow_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="value">すべての値の初期化値</param>
			Task_BodyBlow_Parametor(const float& value);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">移動スピード</param>
			/// <param name="range">移動距離</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">移動スピード</param>
			/// <param name="range">移動距離</param>
			/// <param name="damageData">ダメージデータ</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range, const DamageData& damageData);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">移動スピード</param>
			/// <param name="range">移動距離</param>
			/// <param name="endVelocitySpeed">タックル終了時の減速スピード</param>
			/// <param name="damageData">ダメージデータ</param>
			/// <param name="toughness">強靭度</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range, const float& endVelocitySpeed, const DamageData& damageData, const float& toughness);

			/// <summary>
			/// 移動距離を移動しきったかどうか
			/// </summary>
			/// <returns>移動しきったらtrue</returns>
			bool IsMoveEnd();
		};

		//--------------------------------------------------------------------------------------
		/// ボディーブロータスク
		//--------------------------------------------------------------------------------------
		class Task_BodyBlow : public TaskNodeBase<EnemyBase>
		{
		public:
			using Parametor = Task_BodyBlow_Parametor;

		private:
			bool m_isActive;                   //アクティブかどうか
			ex_weak_ptr<Parametor> m_paramPtr; //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			/// <param name="paramPtr">パラメータ</param>
			Task_BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 移動更新
			/// </summary>
			void MoveUpdate();

			/// <summary>
			/// 回転更新
			/// </summary>
			void RotationUpdate();

			/// <summary>
			/// 移動した距離を計測する
			/// </summary>
			void ElapsedMoveRangeUpdate();  

			/// <summary>
			/// 当たったときのダメ―ジを与えるなどの処理
			/// </summary>
			/// <param name="pair">当たった相手</param>
			void CollisionEnter(const CollisionPair& pair);

			/// <summary>
			/// 当たった相手にダメージを伝える
			/// </summary>
			/// <param name="pair">当たった相手</param>
			void SendDamage(const CollisionPair& pair);

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 終了判断
			/// </summary>
			/// <returns>更新が終了したらtrue</returns>
			bool IsEnd();
		};

	}
}