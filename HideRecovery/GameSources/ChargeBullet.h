/*!
@file ChargeBullet.h
@brief ChargeBullet�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�`���[�W�e
	//--------------------------------------------------------------------------------------
	class ChargeBullet : public BulletBase
	{
	public:
		ChargeBullet(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void Shot(const Vec3& direct) override;
	};

}