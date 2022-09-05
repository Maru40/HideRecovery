/*!
@file LookAtCameraManager.cpp
@brief LookAtCameraManagerなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "LookAtCameraManager.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	LookAtCameraManager::Parametor::Parametor()
		:Parametor(Vec3(0.0f, 1.0f, 0.0f))
	{}

	LookAtCameraManager::Parametor::Parametor(const Vec3& centerOffset)
		:Parametor(4.0f, centerOffset)
	{}

	LookAtCameraManager::Parametor::Parametor(const float& speed, const Vec3& centerOffset)
		:speed(speed), centerOffset(centerOffset)
	{}

	//--------------------------------------------------------------------------------------
	/// カメラの視点管理クラス本体
	//--------------------------------------------------------------------------------------

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
		if (!m_target) { //ターゲットが存在しないなら。
			return;
		}

		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);
		auto camera = GetGameObject()->OnGetDrawCamera();

		auto targetTrans = m_target->GetComponent<Transform>();

		auto position = targetTrans->GetWorldPosition();

		position += m_param.centerOffset;  //playerが足元がpivotになっているためCenter位置を変える。  
		float StartTime = 0.0f;
		float EndTime = 1.0f;
		//auto setPosition = Lerp::CalculateLerp(camera->GetAt(), position, StartTime, EndTime, delta * m_param.speed, Lerp::rate::Linear);
		auto setPosition = position;

		camera->SetAt(setPosition);
	}
}

//endbasecross