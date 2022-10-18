
/*!
@file AroundEyeCheck.cpp
@brief AroundEyeCheckのクラス実体
担当：丸山裕喜
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
		/// パラメータ
		//--------------------------------------------------------------------------------------

		AroundEyeCheck_Parametor::AroundEyeCheck_Parametor()
			:AroundEyeCheck_Parametor(1.0f, 60.0f)
		{}

		AroundEyeCheck_Parametor::AroundEyeCheck_Parametor(const float& speed, const float& aroundDegree)
			:speed(speed), aroundRadian(XMConvertToRadians(aroundDegree))
		{}

		//--------------------------------------------------------------------------------------
		/// 周囲を確認するタスク本体
		//--------------------------------------------------------------------------------------

		AroundEyeCheck::AroundEyeCheck(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<GameObject>(owner), m_paramPtr(paramPtr), m_defaultSpeed(0.0f)
		{}

		void AroundEyeCheck::OnStart() {
			//初期化
			m_index = 0;
			m_directs.clear();
			SettingDirects(); //方向のセッティング

			//ローテーションの設定
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (rotationController) {
				m_defaultSpeed = rotationController->GetSpeed();
				rotationController->SetSpeed(m_paramPtr->speed);
				rotationController->SetDirect(m_directs[m_index]);
			}
			else {
				DebugObject::AddString(L"Task::AroundEyeCheck::OnStart(), RotationControllerが存在しません。");
			}
			
			//速度の減速開始
			auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->StartDeseleration();
			}
		}

		bool AroundEyeCheck::OnUpdate() {
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (!rotationController) {
				return true;  //所持してなかったら終了
			}

			if (!rotationController->IsRotation()) {  //回転が終わっていたら
				NextDirect();
			}

			return IsEnd();
		}

		void AroundEyeCheck::OnExit() {
			//ローテーションの設定
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			if (rotationController) {
				rotationController->SetSpeed(m_defaultSpeed);
			}

			//速度の減速off
			auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->SetIsDeseleration(false);
			}
		}


		void AroundEyeCheck::SettingDirects() {
			auto transform = GetOwner()->GetComponent<Transform>();
			auto forward = transform->GetForward();

			float radians[] = {
				+m_paramPtr->aroundRadian,
				-m_paramPtr->aroundRadian
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
				rotationController->SetDirect(m_directs[m_index]);
			}
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		bool AroundEyeCheck::IsEnd() {
			return m_directs.size() <= m_index; //サイズを超えたら
		}
	}
}

//endbasecross