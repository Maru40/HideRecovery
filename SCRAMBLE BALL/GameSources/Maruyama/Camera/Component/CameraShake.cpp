/*!
@file CameraShake.cpp
@brief CameraShake�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "CameraShake.h"

#include "GameStageBase.h"
#include "Maruyama/Utility/Random.h"
#include "Maruyama/Utility/Utility.h"

#include "Patch/PlayerInputer.h"

#include "Itabashi/InputPlayerController.h"
#include "Patch/SpringArmComponent.h"

namespace basecross {

	//�p�����[�^----------------------------------------------------------------------------------

	CameraShake::Parametor::Parametor()
		:Parametor(0.0f, Vec3(10.0f, 0.0f, 0.0f), 0.25f, 1.0f, maru::DeltaType::Normal)
	{}

	CameraShake::Parametor::Parametor(
		const float& time, 
		const Vec3& powerVec, 
		const float& powerf,
		const float& updateTimeSpeed,
		const maru::DeltaType& deltaType
	):
		time(time), powerVec(powerVec), powerf(powerf), 
		updateTimeSpeed(updateTimeSpeed), deltaType(deltaType),
		centerPosition(Vec3(0.0f))
	{}

	//--------------------------------------------------------------------------------------------

	CameraShake::CameraShake(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_param(Parametor()), m_camera(nullptr), m_timer(new GameTimer(0.0f))
	{}

	void CameraShake::OnUpdate2() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		m_timer->UpdateTimer(m_param.updateTimeSpeed, m_param.deltaType);
		ShakeUpdate();

		if(m_timer->IsTimeUp()) {
			if (m_param.exitFunction) {
				m_param.exitFunction();
				m_param.exitFunction = nullptr;
			}
		}
	}

	void CameraShake::ShakeUpdate() {
		if (!m_camera) {
			return;
		}

		auto camera = m_camera.GetShard();
		auto forward = camera->GetAt() - camera->GetEye();
		forward.y = 0.0f;
		auto powerVec = maru::Utility::ConvertForwardOffset(forward.GetNormalized(), m_param.powerVec);

		auto shakeVec = maru::MyRandom::RandomVector(powerVec).GetNormalized() * m_param.powerf;
		auto position = m_param.centerPosition + shakeVec;

		transform->SetPosition(position);
	}

	//�A�N�Z�b�T----------------------------------------------------------------------------------------------------

	void CameraShake::StartShake(const float& time) {
		StartShake(time, m_param.powerVec);
	}

	void CameraShake::StartShake(const float& time, const Vec3& powerVec) {
		StartShake(time, powerVec, m_param.powerf);
	}

	void CameraShake::StartShake(const float& time, const Vec3& powerVec, const float& powerf) {
		StartShake(time, powerVec, powerf, m_param.updateTimeSpeed);
	}

	void CameraShake::StartShake(const float& time, const Vec3& powerVec, const float& powerf, 
		const float& updateTimeSpeed) 
	{
		StartShake(time, powerVec, powerf, updateTimeSpeed, m_param.deltaType);
	}

	void CameraShake::StartShake(const float& time, const Vec3& powerVec, const float& powerf,
		const float& updateTimeSpeed, const maru::DeltaType& deltaType) 
	{
		StartShake(Parametor(time, powerVec, powerf, updateTimeSpeed, deltaType));
	}

	void CameraShake::StartShake(const float& time, const maru::DeltaType& deltaType) {
		StartShake(time, m_param.powerVec, m_param.powerf, m_param.updateTimeSpeed, deltaType);
	}

	void CameraShake::StartShake(const Parametor& parametor) {
		auto gameStage = GetGameStage();
		if (gameStage == nullptr) {
			return;
		}
		m_camera = gameStage->GetCamera<Camera>();
		m_param = parametor;
		m_param.centerPosition = transform->GetPosition();

		//auto sprintArm = m_camera->GetCameraObject()->GetComponent<SpringArmComponent>(false);
		//sprintArm->SetUpdateActive(false);

		m_timer->ResetTimer(parametor.time);
	}

}