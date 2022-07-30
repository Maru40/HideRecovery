/*!
@file SeekTarget.cpp
@brief SeekTargetLookなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "SeekTarget.h"

#include "VelocityManager.h"
#include "UtilityVelocity.h"

#include "Mathf.h"

//#include "DebugObject.h"

namespace basecross {

	//パラメータ----------------------------------------------------------------------------------

	SeekTarget_Parametor::SeekTarget_Parametor()
		:SeekTarget_Parametor(
			15.0f,                    //アームの長さ
			Vec3(0.0f, 7.0f, -10.0f), //調整用offset 
			SeekType::Lerp)
	{}

	SeekTarget_Parametor::SeekTarget_Parametor(const float& range, const Vec3& offset, const SeekType& seekType)
		:SeekTarget_Parametor(10.0f, range, offset, seekType)
	{}

	SeekTarget_Parametor::SeekTarget_Parametor(const float& speed, const float& range, const Vec3& offset, const SeekType& seekType, const bool isScaleParent)
		:speed(speed), range(range), offset(offset), seekType(seekType), isScaleParent(isScaleParent)
	{}

	//--------------------------------------------------------------------------------------------

	SeekTarget::SeekTarget(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target)
		:SeekTarget(objPtr, target, Parametor())
	{}

	SeekTarget::SeekTarget(const std::shared_ptr<GameObject>& objPtr, 
		const std::shared_ptr<GameObject>& target, 
		const Parametor& parametor
	):
		Component(objPtr), m_target(target), m_param(parametor)
	{}

	void SeekTarget::OnCreate() {
		if (m_target) {
			transform->SetPosition(CalculatePosition());
		}
	}

	void SeekTarget::OnUpdate() {
		ChaseUpdate();
	}

	void SeekTarget::ChaseUpdate() {
		if (!m_target) { //ターゲットがnullなら処理をしない
			return;
		}

		auto position = CalculatePosition();
		
		switch (m_param.seekType) //シークタイプによってポジションを変える。
		{  
		case SeekType::Normal:
			NormalChase(position);
			break;

		case SeekType::Lerp:
			LerpChase(position);
			break;
			
		case SeekType::VelocitySeek:
			VelocityChase(position);
			break;

		case SeekType::VelcoityArrive:
			VelocityChase(position);
			break;

		case SeekType::VelocityNearSeekFarArrive:
			VelocityChase(position);
			break;
		}
	}

	Vec3 SeekTarget::CalculatePosition() {
		return GetWorldPosition();
	}

	void SeekTarget::NormalChase(const Vec3& basePosition) {
		transform->SetPosition(basePosition);
	}

	void SeekTarget::LerpChase(const Vec3& basePosition) {
		auto delta = App::GetApp()->GetElapsedTime();
		auto position = Lerp::CalculateLerp(transform->GetPosition(), basePosition, 0.0f, 1.0f, delta * m_param.speed, Lerp::rate::Linear);
		transform->SetPosition(position);
	}

	void SeekTarget::VelocityChase(const Vec3& basePosition) {
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (!velocityManager) {
			return;
		}

		auto toVec = basePosition - transform->GetPosition();
		auto velocity = velocityManager->GetVelocity();
		Vec3 force(0.0f);

		switch (m_param.seekType)
		{
		case SeekType::VelocitySeek:
			force = UtilityVelocity::CalucSeekVec(velocity, toVec, m_param.speed);
			break;

		case SeekType::VelcoityArrive:
			force = UtilityVelocity::CalucArriveVec(velocity, toVec, m_param.speed);
			break;

		case SeekType::VelocityNearSeekFarArrive :
			force = UtilityVelocity::CalucNearArriveFarSeek(velocity, toVec, m_param.speed, 0.25f);
			break;
		}

		velocityManager->AddForce(force);
	}

	Vec3 SeekTarget::CalculateOffset() const {
		auto offset = m_param.offset;

		if (m_param.isScaleParent) {
			auto targetTrans = m_target->GetComponent<Transform>();
			auto targetScale = targetTrans->GetScale();

			offset = XMVectorMultiply(offset, targetScale);
		}

		return offset;
	}

}

//endbasecross