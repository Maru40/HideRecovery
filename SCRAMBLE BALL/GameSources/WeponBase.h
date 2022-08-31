﻿
/*!
@file WeponBase.h
@brief WeponBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BulletObjectBase;

	//--------------------------------------------------------------------------------------
	/// ウェポンのパラメータ
	//--------------------------------------------------------------------------------------
	struct WeponBase_Parametor {
		float weight;				//重さ
		float shotIntervalTime;		//連射速度
		Vec3 bulletInstanceOffset;	//バレットを生成する位置のオフセット

		WeponBase_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// ウェポンの基底クラス
	//--------------------------------------------------------------------------------------
	class WeponBase : public Component
	{
	public:
		using Parametor = WeponBase_Parametor;

	private:
		Parametor m_param;

	public:
		WeponBase(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr)
		{}

		virtual ~WeponBase() = default;

		/// <summary>
		/// 弾を撃つ
		/// </summary>
		/// <param name="direct">撃つ方向</param>
		/// <returns>撃った弾のオブジェクト</returns>
		virtual std::shared_ptr<BulletObjectBase> Shot(const Vec3& direct) = 0;

		/// <summary>
		/// 弾の生成
		/// </summary>
		template<class BulletType, class... Ts>
		std::shared_ptr<BulletType> InstantiateBullet(const Vec3& position, const Quat& quat, Ts... params) {
			auto bulletObject = GetStage()->Instantiate<BulletType>(position, quat, params...);

			return bulletObject;
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// パラメータの設定
		/// </summary>
		/// <param name="parametor">パラメータ</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// パラメータの取得
		/// </summary>
		/// <returns>パラメータ</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// 武器の重さの設定
		/// </summary>
		/// <param name="weight">武器の重さ</param>
		void SetWeight(const float weight) noexcept { m_param.weight = weight; }

		/// <summary>
		/// 重さの取得
		/// </summary>
		/// <returns>重さ</returns>
		float GetWeight() const noexcept { return m_param.weight; }

		void SetShotIntervalTime(const float time) { m_param.shotIntervalTime = time; }

		float GetShotIntervalTime() const noexcept { return m_param.shotIntervalTime; }

		/// <summary>
		/// バレットを生成する位置のオフセットの設定
		/// </summary>
		/// <param name="offset">バレットを生成する位置のオフセット</param>
		void SetBulletInstanceOffset(const Vec3& offset) noexcept { m_param.bulletInstanceOffset = offset; }

		/// <summary>
		/// バレットを生成する位置のオフセットの取得
		/// </summary>
		/// <returns>バレットを生成する位置のオフセット</returns>
		Vec3 GetBulletInstanceOffset() const noexcept { return m_param.bulletInstanceOffset; }

	};

}