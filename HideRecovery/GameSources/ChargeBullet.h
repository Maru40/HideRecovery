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
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	///	�`���[�W�e
	//--------------------------------------------------------------------------------------
	class ChargeBullet : public BulletBase
	{
	public:
		ChargeBullet(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) override;

		void OnCollisionEnter(const CollisionPair& pair) override;
	};

}