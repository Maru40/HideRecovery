/*!
@file BehaviorTree.cpp
@brief BehaviorTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTree.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			ObserveStackData::ObserveStackData(
				const int stackIndex,
				const std::shared_ptr<I_Node>& node
			):
				stackIndex(stackIndex),
				node(node)
			{
				this->state = node->GetState();	//スタックされた時のステートを取得
			}

		}

	}
}