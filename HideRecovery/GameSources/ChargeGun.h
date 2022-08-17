
/*!
@file ChargeGun.h
@brief ChargeGun�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "WeponBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class ChargeBulletObject;

	//--------------------------------------------------------------------------------------
	///	�`���[�W�e
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
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		void PlayAnimation();
	};

}