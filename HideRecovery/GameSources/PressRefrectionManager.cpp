/*!
@file PressRefrectionManager.cpp
@brief PressRefrectionManagerのクラス実体
担当：丸山裕喜
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
	/// パラメータ
	//--------------------------------------------------------------------------------------

	PressRefrectionManager::Parametor::Parametor()
		:Parametor(Vec3(0.0f), 1.0f)
	{}

	PressRefrectionManager::Parametor::Parametor(const Vec3& velocity, const float& deselerationPower)
		:velocity(velocity),deselerationPower(deselerationPower)
	{}

	//--------------------------------------------------------------------------------------
	/// 潰す反射管理
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
			DebugObject::AddString(L"PressRefrectionManager::StartReflection: VelocityManagerが存在しません。");
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