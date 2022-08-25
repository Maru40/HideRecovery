
/*!
@file ChargeGun.h
@brief ChargeGunなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "WeponBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class ChargeBulletObject;
	class UseWepon;
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	チャージ銃
	//--------------------------------------------------------------------------------------
	class ChargeGun : public WeponBase
	{
		std::weak_ptr<UseWepon> m_useWepon;

		std::unique_ptr<GameTimer> m_timer;

	public:
		ChargeGun(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

		std::shared_ptr<BulletObjectBase> Shot(const Vec3& direct) override;

	private:
		/// <summary>
		/// アニメーションの更新
		/// </summary>
		void UpdateAnimation();

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		void PlayAnimation();

		/// <summary>
		/// 撃つ方向の修正
		/// </summary>
		void RevisionShotDirection();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 撃てる状態かどうか
		/// </summary>
		/// <returns>撃てる状態ならtrue</returns>
		bool IsShot() const;
	};

}