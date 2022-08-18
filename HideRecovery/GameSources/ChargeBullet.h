﻿/*!
@file ChargeBullet.h
@brief ChargeBulletなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	///	チャージ弾
	//--------------------------------------------------------------------------------------
	class ChargeBullet : public BulletBase
	{
		std::vector<std::function<void(const std::shared_ptr<GameObject>&)>> m_destroyEventFuncs;

		int m_instanceId = -1;

	public:
		ChargeBullet(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) override;

		void OnCollisionEnter(const CollisionPair& pair) override;

		void AddDestroyEventFunc(const std::function<void(const std::shared_ptr<GameObject>&)>& destroyEventFunc)
		{
			m_destroyEventFuncs.push_back(destroyEventFunc);
		}
		
		void SetInstanceId(int instanceId) { m_instanceId = instanceId; }

		int GetInstanceId() const { return m_instanceId; }
	};

}