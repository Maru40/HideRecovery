/*!
@file   SyncObject.cpp
@brief  オブジェクトの位置を同期するコンポーネント実体
*/

#include "stdafx.h"
#include "SyncObject.h"
#include "../Utility/Utility.h"

namespace basecross {
	SyncObject::SyncObject(const shared_ptr<GameObject>& owner)
		:SyncObject(owner, Vec3(0))
	{}

	SyncObject::SyncObject(const shared_ptr<GameObject>& owner, const Vec3& offset)
		: Component(owner), m_offsetPosition(offset)
	{}

	void SyncObject::OnCreate() {
	}
	void SyncObject::OnUpdate() {
		if (!m_targetTrans)
			return;

		auto pos = Utility::ParentRelativePositionToWorldPosition(m_targetTrans, m_offsetPosition);
		transform->SetPosition(pos);
	}
}