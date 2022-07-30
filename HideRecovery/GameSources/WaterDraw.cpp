/*!
@file WaterDraw.cpp
@brief WaterDrawのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "WaterDraw.h"

#include "WaterBodyObject.h"
#include "LaserBase.h"

#include "WaterAuraObject.h"

#include "ParticleManager.h"
#include "ParticleColliderManager.h"

#include "RangeDestroyManager.h"
#include "UtilityObstacle.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	WaterDraw_Parametor::WaterDraw_Parametor() :
		particleCollisionManagerParam(new ParticleColliderManager_Parametor())
	{}

	//--------------------------------------------------------------------------------------
	/// 水圧ジェット本体
	//--------------------------------------------------------------------------------------

	WaterDraw::WaterDraw(const std::shared_ptr<GameObject>& objPtr) :
		BulletBase(objPtr), 
		m_param(Parametor()),
		m_body(nullptr),
		m_boardAura(nullptr),
		m_particleManager(nullptr)
	{}

	void WaterDraw::OnCreate() {
		CreatePartilceManager();

		SetBodyPosition();

		auto param = m_param.particleCollisionManagerParam;
		param->offset = Vec3(+1.0f, -0.5f, 0.0f);
		GetGameObject()->AddComponent<ParticleColliderManager>(*param.get());
	}

	void WaterDraw::OnUpdate() {
		SetBodyPosition();
		SetParticleCollisionLength();
	}

	void WaterDraw::OnDestroy() {
		GetStage()->RemoveGameObject<GameObject>(m_body.GetShard());
		GetStage()->RemoveGameObject<GameObject>(m_boardAura.GetShard());
	}

	void WaterDraw::CreatePartilceManager() {
		auto object = GetStage()->Instantiate<GameObject>(transform->GetWorldPosition(), transform->GetQuaternion());
		auto vertexParam = Builder::VertexPCTParametor(Vec2(256.0f), L"Point_TX");
		vertexParam.color = Col4(1.0f, 1.0f, 1.0f, 0.3f);
		vertexParam.size = 0.3f;

		auto param = maru::ParticleManager_Parametor(vertexParam);
		param.isRayHitDestory = true;
		
		auto particleManager = object->AddComponent<maru::ParticleManager>(param);

		m_particleManager = particleManager;
	}

	void WaterDraw::Shot(const Vec3& direct) {
		transform->SetForward(direct);
	}

	void WaterDraw::ShotEnd() {
		GetStage()->RemoveGameObject<GameObject>(m_particleManager->GetGameObject());

		auto collisionManager = GetGameObject()->GetComponent<ParticleColliderManager>(false);
		if (collisionManager) {
			collisionManager->SetLengthUpdateType(LengthUpdateType::Smaller);
		}
	}

	void WaterDraw::SetBodyPosition() {
		if (m_particleManager) {
			auto prticleTrans = m_particleManager->GetGameObject()->GetComponent<Transform>();
			prticleTrans->SetForward(transform->GetForward());
			prticleTrans->SetPosition(transform->GetWorldPosition());
		}
	}

	void WaterDraw::SetParticleCollisionLength() {
		auto particleCollisionManager = GetGameObject()->GetComponent<ParticleColliderManager>(false);
		if (!particleCollisionManager) {
			return;
		}

		constexpr float SearchRange = 50.0f;
		auto forward = transform->GetForward();
		auto startPosition = transform->GetWorldPosition();
		auto endPosition = startPosition + (forward * SearchRange);

		auto rayHitObject = maru::UtilityObstacle::FindRayHitNearObstacle(
			startPosition, endPosition,
			GetStage()->GetGameObjectVec(),
			maru::UtilityObstacle::DEFAULT_OBSTACLE_TAGS
		);

		if(!rayHitObject){
			return;
		}

		auto toRayHitObjectVec = maru::Utility::CalcuToTargetVec(GetGameObject(), rayHitObject);
		const float& maxLength = m_param.particleCollisionManagerParam->maxLength;

		//ヒットしたらその対象までの長さ、そうでないなら、最大の長さ
		float setLength = toRayHitObjectVec.length() < maxLength ? toRayHitObjectVec.length() : maxLength;
		particleCollisionManager->SetMaxLength(setLength);
	}

}