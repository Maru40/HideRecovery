
/*!
@file AroundEyeCheck.cpp
@brief AroundEyeCheck�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Task_AroundEyeCheck.h"

#include "Maruyama/Utility/Component/RotationController.h"
#include "VelocityManager.h"

#include "Maruyama/DebugClass/Object/DebugObject.h"

#include "Maruyama/Utility/Mathf.h"

namespace basecross {
	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		AroundEyeCheck_Parametor::AroundEyeCheck_Parametor()
			:AroundEyeCheck_Parametor(1.0f, 60.0f)
		{}

		AroundEyeCheck_Parametor::AroundEyeCheck_Parametor(const float& speed, const float& aroundDegree)
			:speed(speed), aroundRadian(XMConvertToRadians(aroundDegree))
		{}

		//--------------------------------------------------------------------------------------
		/// ���͂��m�F����^�X�N�{��
		//--------------------------------------------------------------------------------------

		AroundEyeCheck::AroundEyeCheck(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<GameObject>(owner), m_paramPtr(paramPtr), m_defaultSpeed(0.0f)
		{}

		void AroundEyeCheck::OnStart() {
			//������
			m_index = 0;
			m_directs.clear();
			SettingDirects(); //�����̃Z�b�e�B���O

			//���[�e�[�V�����̐ݒ�
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (rotationController) {
				m_defaultSpeed = rotationController->GetSpeed();
				rotationController->SetSpeed(m_paramPtr.lock()->speed);
				rotationController->SetDirection(m_directs[m_index]);
			}
			else {
				DebugObject::AddString(L"Task::AroundEyeCheck::OnStart(), RotationController�����݂��܂���B");
			}
			
			//���x�̌����J�n
			auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->StartDeseleration();
			}
		}

		bool AroundEyeCheck::OnUpdate() {
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (!rotationController) {
				return true;  //�������ĂȂ�������I��
			}

			if (!rotationController->IsRotation()) {  //��]���I����Ă�����
				NextDirect();
			}

			return IsEnd();
		}

		void AroundEyeCheck::OnExit() {
			//���[�e�[�V�����̐ݒ�
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (rotationController) {
				rotationController->SetSpeed(m_defaultSpeed);
			}

			//���x�̌���off
			auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->SetIsDeseleration(false);
			}
		}


		void AroundEyeCheck::SettingDirects() {
			auto transform = GetOwner()->GetComponent<Transform>();
			auto forward = transform->GetForward();

			float radians[] = {
				+m_paramPtr.lock()->aroundRadian,
				-m_paramPtr.lock()->aroundRadian
			};

			for (const auto& radian : radians) {
				m_directs.push_back(maru::Mathf::Mat4RotationAxisY(forward, radian));
			}
		}

		void AroundEyeCheck::NextDirect() {
			m_index++;
			if (IsEnd()) {
				return;
			}

			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (rotationController) {
				rotationController->SetDirection(m_directs[m_index]);
			}
		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		bool AroundEyeCheck::IsEnd() {
			return m_directs.size() <= m_index; //�T�C�Y�𒴂�����
		}
	}
}

//endbasecross