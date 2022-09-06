/*!
@file BulletBase.h
@brief BulletBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Damaged.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct BulletBase_Parametor {
		Vec3 moveDirect;		//移動方向
		float moveSpeed;		//移動スピード
		float maxRange;			//移動最大距離
		DamageData damageData;	//ダメージデータ

		BulletBase_Parametor();
	};

	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラス
	//--------------------------------------------------------------------------------------
	class BulletBase : public Component
	{
	public:
		using Parametor = BulletBase_Parametor;

	private:
		Parametor m_param;					//パラメータ

		std::weak_ptr<GameObject> m_owner;	//この弾の所有者

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		BulletBase(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		/// <summary>
		/// 撃つ処理
		/// </summary>
		/// <param name="owner">撃った者</param>
		/// <param name="direct">撃つ方向</param>
		virtual void Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) {}

		/// <summary>
		/// 撃つ処理が終了したタイミングで呼ぶ処理
		/// </summary>
		virtual void ShotEnd() {}

	private:
		/// <summary>
		/// 移動更新
		/// </summary>
		void MoveUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// 移動方向の設定
		/// </summary>
		/// <param name="direct">移動方向</param>
		void SetMoveDirect(const Vec3& direct) noexcept { m_param.moveDirect = direct; }

		/// <summary>
		/// 移動方向の取得
		/// </summary>
		/// <returns>移動方向</returns>
		Vec3 GetMoveDirect() const noexcept { return m_param.moveDirect; }

		void SetMoveSpeed(const float speed) noexcept { m_param.moveSpeed = speed; }

		/// <summary>
		/// 移動最大距離の設定
		/// </summary>
		/// <param name="range">移動最大距離</param>
		void SetMaxRange(const float range) noexcept { m_param.maxRange = range; }

		/// <summary>
		/// 移動最大距離の取得
		/// </summary>
		/// <returns>移動最大距離</returns>
		float GetMaxRange() const noexcept { return m_param.maxRange; }

		/// <summary>
		/// ダメージデータの設定
		/// </summary>
		/// <param name="data">ダメージデータ</param>
		void SetDamageData(const DamageData& data) noexcept { m_param.damageData = data; }

		/// <summary>
		/// ダメージデータの取得
		/// </summary>
		/// <returns>ダメージデータ</returns>
		DamageData GetDamageData() const noexcept { return m_param.damageData; }

		/// <summary>
		/// 所有者の設定
		/// </summary>
		/// <param name="owner">所有者</param>
		void SetOwner(const std::shared_ptr<GameObject>& owner) noexcept { m_owner = owner; }

		/// <summary>
		/// 所有者の取得
		/// </summary>
		/// <returns>所有者</returns>
		std::shared_ptr<GameObject> GetOwner() const { return m_owner.lock(); }
	};

}

//endbasecross