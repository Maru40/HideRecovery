/*!
@file LookAtCameraManager.cpp
@brief LookAtCameraManager�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "LookAtCameraManager.h"

#include "TimeHelper.h"

namespace basecross {

	//�p�����[�^----------------------------------------------------------------------------------

	LookAtCameraManager::Parametor::Parametor()
		:Parametor(Vec3(0.0f, 0.5f, 0.0f))
	{}

	LookAtCameraManager::Parametor::Parametor(const Vec3& centerOffset)
		:Parametor(4.0f, centerOffset)
	{}

	LookAtCameraManager::Parametor::Parametor(const float& speed, const Vec3& centerOffset)
		:speed(speed), centerOffset(centerOffset)
	{}

	//--------------------------------------------------------------------------------------------

	LookAtCameraManager::LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr)
		:LookAtCameraManager(objPtr, nullptr)
	{}

	LookAtCameraManager::LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target)
		:LookAtCameraManager(objPtr, target, Parametor())
	{}

	LookAtCameraManager::LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, 
		const std::shared_ptr<GameObject>& target, 
		const Parametor& param
	):
		Component(objPtr), m_target(target), m_param(param)
	{}

	void LookAtCameraManager::OnUpdate2() {
		LookAtUpdate();
	}

	void LookAtCameraManager::LookAtUpdate()
	{
		if (!m_target) { //�^�[�Q�b�g�����݂��Ȃ��Ȃ�B
			return;
		}

		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);
		auto camera = GetGameObject()->OnGetDrawCamera();

		auto targetTrans = m_target->GetComponent<Transform>();

		auto position = targetTrans->GetWorldPosition();

		position += m_param.centerOffset;  //player��������pivot�ɂȂ��Ă��邽��Center�ʒu��ς���B  
		float StartTime = 0.0f;
		float EndTime = 1.0f;
		auto setPosition = Lerp::CalculateLerp(camera->GetAt(), position, StartTime, EndTime, delta * m_param.speed, Lerp::rate::Linear);

		camera->SetAt(setPosition);
	}
}

//endbasecross