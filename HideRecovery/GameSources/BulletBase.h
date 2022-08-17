/*!
@file BulletBase.h
@brief BulletBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct BulletBase_Parametor {
		Vec3 moveDirect;	//移動方向
		float moveSpeed;	//移動スピード
		float maxRange;		//移動最大距離

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
		Parametor m_param;	//パラメータ

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
		/// <param name="direct">撃つ方向</param>
		virtual void Shot(const Vec3& direct) {}

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
	};

}

//endbasecross