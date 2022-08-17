/*!
@file ChargeBullet.h
@brief ChargeBulletなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	チャージ弾
	//--------------------------------------------------------------------------------------
	class ChargeBullet : public BulletBase
	{
	public:
		ChargeBullet(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void Shot(const Vec3& direct) override;
	};

}