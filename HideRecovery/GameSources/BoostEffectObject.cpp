/*!
@file BoostEffectObject.cpp
@brief BoostEffectObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BoostEffectObject.h"

#include "BoostCenterObject.h"

#include "BoardObject.h"
#include "BuilderVertexPCT.h"

#include "BillBoard_Ex.h"

#include "TimerDestroy.h"

namespace basecross {

	BoostEffectObject::BoostEffectObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage), m_center(nullptr)
	{}

	void BoostEffectObject::OnCreate() {
		auto param = Builder::VertexPCTParametor(Vec2(512.0f), L"Boost_TX");
		auto center = GetStage()->AddGameObject<BoardObject>(param);
		center->AddComponent<BillBoard_Ex>();

		ex_weak_ptr<GameObject> tempCenter = center;
		m_objects.push_back(tempCenter);

		const float Time = 1.0f;
		AddComponent<TimerDestroy>(Time);
	}

	void BoostEffectObject::OnUpdate() {
		for (auto& object : m_objects) {
			auto objTrans = object->GetComponent<Transform>();
			objTrans->SetPosition(transform->GetPosition());
		}
	}

	void BoostEffectObject::OnDestroy() {
		for (auto& object : m_objects) {
			GetStage()->RemoveGameObject<GameObject>(object.GetShard());
		}
	}

}