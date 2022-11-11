/*!
@file RotationController.cpp
@brief RotationController����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "RotationController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�p�����[�^
	//--------------------------------------------------------------------------------------

	RotationController::Parametor::Parametor()
		:Parametor(7.5f)
	{}

	RotationController::Parametor::Parametor(const float& speed, const bool isUseY)
		:Parametor(speed, Vec3(0.0f), isUseY)
	{}

	RotationController::Parametor::Parametor(const float& speed, const Vec3& direction, const bool isUseY)
		:speed(speed),direction(direction),isUseY(isUseY)
	{}

	//--------------------------------------------------------------------------------------
	///	��]�R���g���[���{��
	//--------------------------------------------------------------------------------------

	RotationController::RotationController(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void RotationController::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		auto obj = GetGameObject();
		auto objTrans = obj->GetComponent<Transform>();

		auto direct = m_param.direction;
		if (!m_param.isUseY) {
			direct.y = 0.0f;
		}

		auto utilPtr = obj->GetBehavior<UtilBehavior>();
		utilPtr->RotToHead(direct, m_param.speed * delta);
	}

	bool RotationController::IsRotation() {
		float newDot = dot(transform->GetForward(), m_param.direction.GetNormalized());
		float angleRad = abs(acosf(newDot));
		constexpr float AngleBettween = XMConvertToRadians(5.0f);

		//�p�x���w�肵�����傫���������]��
		return AngleBettween < angleRad;
	}
}

//endbasecorss