/*!
@file PressRefrectionManager.cpp
@brief PressRefrectionManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "PressRefrectionManager.h"
#include "PressCenterManager.h"

#include "VelocityManager.h"
#include "Pusher.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	PressRefrectionManager::Parametor::Parametor()
		:Parametor(Vec3(0.0f), 1.0f)
	{}

	PressRefrectionManager::Parametor::Parametor(const Vec3& velocity, const float& deselerationPower)
		:velocity(velocity),deselerationPower(deselerationPower)
	{}

	//--------------------------------------------------------------------------------------
	/// �ׂ����ˊǗ�
	//--------------------------------------------------------------------------------------

	PressRefrectionManager::PressRefrectionManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	Vec3 PressRefrectionManager::CalcuReflectionVec(const PressData& data) {
		Vec3 reVec;

		return reVec;
	}

	void PressRefrectionManager::StartReflection(const PressData& data, const float& deselerationPower) {
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityManager == nullptr) {
			DebugObject::AddString(L"PressRefrectionManager::StartReflection: VelocityManager�����݂��܂���B");
			return;
		}

		velocityManager->ResetAll();
		velocityManager->SetVelocity(data.missData.refrectionVec);
		velocityManager->StartDeseleration(deselerationPower);

		auto pusher = data.GetPusher();
		if (pusher) {
			pusher->PushBack(-data.missData.pusherVec);
		}
	}
}

//endbasecross