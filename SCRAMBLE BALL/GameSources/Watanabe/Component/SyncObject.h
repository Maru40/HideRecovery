/*!
@file   SyncObject.h
@brief  オブジェクトの位置を同期するコンポーネント
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class SyncObject :public Component {
		Vec3 m_offsetPosition;
		shared_ptr<Transform> m_targetTrans;
	public:
		SyncObject(const shared_ptr<GameObject>& owner);
		SyncObject(const shared_ptr<GameObject>& owner, const Vec3& offset);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void SetOffsetPosition(const Vec3& offset) { m_offsetPosition = offset; }
		void SetTarget(const shared_ptr<Transform>& transPtr) { m_targetTrans = transPtr; }
	};
}
