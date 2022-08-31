
/*!
@file ReactiveProperty.cpp
@brief ReactivePropertyのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CameraRayAlphaManager.h"

#include "MaruHitHelper.h"

namespace basecross {

	CameraRayAlphaManager::CameraRayAlphaManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const float& range) :
		Component(objPtr), m_target(target), m_range(range)
	{}

	void CameraRayAlphaManager::OnCreate() {

	}

	void CameraRayAlphaManager::OnUpdate() {
		if (!m_target) {
			return;
		}

		auto selfPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		auto targetPos = m_target->GetComponent<Transform>()->GetPosition();

		const float Range = m_range;
		auto objs = GetStage()->GetGameObjectVec();
		for (auto& obj : objs)
		{
			if (obj == m_target.GetShard()) {  //相手がターゲットだったら処理を飛ばす。
				continue;
			}

			auto col = obj->GetComponent<CollisionObb>(false);
			if (col) {  //コリジョンを持っていたら。

				if (HitTest::SEGMENT_OBB(selfPos, targetPos, col->GetObb())) {  //コリジョンに当たっていたら。
					auto hitData = maru::HitHelper::ClosestPtPointOBB(selfPos, col);
					auto toHitPoint = hitData.point - selfPos;
					if (toHitPoint.length() < Range) {
						obj->SetDrawActive(false);
					}
					else {
						obj->SetDrawActive(true);
					}
				}
				else {
					obj->SetDrawActive(true);
				}
			}

		}
	}

}