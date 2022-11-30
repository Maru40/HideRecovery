/*!
@file BehaviorTesterObject.cpp
@brief BehaviorTesterObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTesterObject.h"
#include "../Tester_Behavior.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			BehaviorTesterObject::BehaviorTesterObject(const std::shared_ptr<Stage>& stage):
				GameObject(stage)
			{}

			void BehaviorTesterObject::OnCreate() {
				AddComponent<TesterTree>();
			}

		}
	}
}