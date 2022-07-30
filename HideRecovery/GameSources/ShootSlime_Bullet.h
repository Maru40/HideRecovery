/*!
@file ShootSlime_Bullet.h
@brief ShootSlime_Bulletなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライムの弾パラメータ
			//--------------------------------------------------------------------------------------
			struct Bullet_Parametor
			{
				DamageData damageData; //ダメージデータ
				float destroyTime;     //弾けてから消えるまでの時間

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Bullet_Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="damageData">ダメージデータ</param>
				/// <param name="destroyTime">弾けてから消えるまでの時間</param>
				Bullet_Parametor(const DamageData& damageData, const float& destroyTime);
			};

			//--------------------------------------------------------------------------------------
			/// ショットスライムの弾
			//--------------------------------------------------------------------------------------
			class Bullet : public basecross::BulletBase
			{
			public:
				using Parametor = Bullet_Parametor;

			private:
				Parametor m_param; //パラメータ
				bool m_isActive;   //アクティブかどうか

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				Bullet(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				/// <param name="parametor">パラメータ</param>
				Bullet(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

				void OnCollisionEnter(const CollisionPair& pair) override;

			private:
				/// <summary>
				/// あたり判定を取る状態かどうか
				/// </summary>
				/// <param name="pair">当たった相手</param>
				/// <returns>あたり判定を取るならtrue</returns>
				bool IsCollision(const CollisionPair& pair);

			public:

				/// <summary>
				/// 撃つ処理
				/// </summary>
				/// <param name="speed">移動スピード</param>
				/// <param name="direct">移動方向</param>
				void Shoot(const float& speed, const Vec3& direct);

				/// <summary>
				/// 壊れる時の処理
				/// </summary>
				void DestroyProcess();
			};

		}

	}
}