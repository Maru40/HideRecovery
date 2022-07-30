/*!
@file LaserCollider.cpp
@brief LaserCollider‚ÌƒNƒ‰ƒXÀ‘Ì
’S“–FŠÛR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "LaserCollider.h"

#include "LaserBase.h"
#include "WaterCollisionManager.h"

namespace basecross {

	LaserCollider::LaserCollider(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_collisionObject(nullptr)
	{}

	void LaserCollider::OnCreate() {
		auto collisionObject = GetStage()->AddGameObject<GameObject>();
		collisionObject->AddComponent<WaterCollisionManager>();
		auto collision = collisionObject->AddComponent<CollisionObb>();
		collision->AddExcludeCollisionTag(L"Player");
		collision->SetAfterCollision(AfterCollision::None);
		//collision->SetDrawActive(true);

		m_collisionObject = collisionObject;
	}

	void LaserCollider::OnUpdate() {
		m_collisionObject->GetComponent<Transform>()->SetPosition(transform->GetPosition());

		HeightUpdate();
	}

	void LaserCollider::OnDestroy() {
		GetStage()->RemoveGameObject<GameObject>(m_collisionObject.GetShard());
	}

	void LaserCollider::SetColliderDrawActive(const bool isDraw) {
		auto collision = m_collisionObject->GetComponent<CollisionObb>();
		collision->SetDrawActive(true);
	}

	void LaserCollider::HeightUpdate() {
		auto laser = GetGameObject()->GetComponent<LaserBaseDraw>(false);
		if (!laser) {
			return;
		}

		auto colTrans = m_collisionObject->GetComponent<Transform>();

		float width = laser->GetWidth();
		auto setScale = Vec3(width, width, laser->GetLength());
		colTrans->SetScale(setScale);

		colTrans->SetForward(laser->GetForward());

		auto setPosition = transform->GetWorldPosition();
		float halfLength = laser->GetLength() * 0.5f;
		//Smaller‚È‚ç‹t
		auto forward = laser->IsSmaller() ? -laser->GetForward() : laser->GetForward();
		setPosition += forward.GetNormalized() * halfLength;
		colTrans->SetPosition(setPosition);
	}
}