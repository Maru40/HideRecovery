
/*!
@file Task_GoalAnimation.cpp
@brief Task_GoalAnimation�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_GoalAnimation.h"

#include "VelocityManager.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/Mathf.h"

namespace basecross {
	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �W�����v��̌��̈ʒu�ɖ߂鏈���̃p�����[�^
		//--------------------------------------------------------------------------------------

		ReturnJump_Parametor::ReturnJump_Parametor() :
			returnDirect(Vec3(0.0f)),
			jumpRad(XMConvertToRadians(80.0f)),
			jumpUpPower(355.0f),
			jumpHorizontalPower(4.5f)
		{}

		//--------------------------------------------------------------------------------------
		/// �W�����v��̌��̈ʒu�ɖ߂鏈��
		//--------------------------------------------------------------------------------------

		ReturnJump::ReturnJump(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr) :
			TaskNodeBase(objPtr),
			m_param(paramPtr)
		{}

		void ReturnJump::OnStart() {
			if (auto gravity = GetOwner()->GetComponent<Gravity>(false)) {
				gravity->SetGravityVerocityZero();
			}

			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				velocityManager->SetUpdateActive(true);
				velocityManager->ResetAll();
				velocityManager->AddForce(CalculateJumpVec().GetNormalized() * GetParametor()->jumpHorizontalPower);
				velocityManager->AddForce(Vec3::Up() * GetParametor()->jumpUpPower);
			}
		}

		bool ReturnJump::OnUpdate() {
			return true;
		}

		void ReturnJump::OnExit() {
			if (auto gravity = GetOwner()->GetComponent<Gravity>(false)) {
				gravity->SetUpdateActive(true);
			}
		}

		Vec3 ReturnJump::CalculateJumpVec() {
			Vec3 direction = GetParametor()->returnDirect;
			direction.y = 0.0f;

			Vec3 right = GetOwner()->GetComponent<Transform>()->GetRight();
			float jumpRad = GetParametor()->jumpRad;
			auto resultVec = maru::Mathf::RotationRadianVec(direction, jumpRad, right);
			resultVec.y = 0.0f;

			return resultVec;
		}
	}
}