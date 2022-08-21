
/*!
@file Task_GoalAnimation.cpp
@brief Task_GoalAnimationクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_GoalAnimation.h"

#include "VelocityManager.h"
#include "MaruUtility.h"
#include "Mathf.h"

namespace basecross {
	namespace Task {

		ReturnJump_Parametor::ReturnJump_Parametor() :
			returnDirect(Vec3(0.0f)),
			jumpRad(XMConvertToRadians(65.0f)),
			jumpUpPower(850.0f),
			jumpHorizontalPower(450.0f)
		{}

		ReturnJump::ReturnJump(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr) :
			TaskNodeBase(objPtr),
			m_param(paramPtr)
		{}

		void ReturnJump::OnStart() {
			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				velocityManager->AddForce(CalculateJumpVec().GetNormalized() * GetParametor()->jumpHorizontalPower);
				velocityManager->AddForce(Vec3::Up() * GetParametor()->jumpUpPower);
			}
		}

		bool ReturnJump::OnUpdate() {
			return true;
		}

		void ReturnJump::OnExit() {
			if (auto gravity = GetOwner()->GetComponent<Gravity>(false)) {
				gravity->SetGravityVerocityZero();
				gravity->SetUpdateActive(true);
			}
		}

		Vec3 ReturnJump::CalculateJumpVec() {
			Vec3 direction = GetParametor()->returnDirect;
			direction.y = 0;

			Vec3 right = GetOwner()->GetComponent<Transform>()->GetRight();
			auto resultVec = maru::Mathf::RotationRadianVec(direction, GetParametor()->jumpRad, right);

			return resultVec;
		}
	}
}