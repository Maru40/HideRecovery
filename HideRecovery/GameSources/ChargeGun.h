
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

	//--------------------------------------------------------------------------------------
	///	チャージ銃
	//--------------------------------------------------------------------------------------
	class ChargeGun : public WeponBase<ChargeBulletObject>
	{
	public:
		ChargeGun(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

		std::shared_ptr<ChargeBulletObject> Shot(const Vec3& direct) override;

	private:
		/// <summary>
		/// Shotアニメーション中の更新処理
		/// </summary>
		void UpdateShotAnimation();

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		void PlayAnimation();

		/// <summary>
		/// 撃つ方向の修正
		/// </summary>
		void RevisionShotDirection();
	};

}