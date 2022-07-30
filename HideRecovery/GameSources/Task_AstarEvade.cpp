/*!
@file Task_AstarEvade.cpp
@brief Task_AstarEvadeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "Task_AstarEvade.h"

#include "TargetManager.h"
#include "UtilityVelocity.h"
#include "VelocityManager.h"
#include "RotationController.h"

namespace basecross {

	namespace Task {

		AstarEvade::AstarEvade(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
		{}

		void AstarEvade::OnStart() {

		}

		bool AstarEvade::OnUpdate() {
			return true;
		}

		void AstarEvade::OnExit() {

		}

	}
}