
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

		void OnUpdate() override;

		void Shot(const Vec3& direct) override;

	private:

		void UpdateAnimation();

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		void PlayAnimation();
	};

}