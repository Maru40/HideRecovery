/*!
@file BehaviorTree.cpp
@brief BehaviorTree�Ȃǎ���
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
				this->state = node->GetState();	//�X�^�b�N���ꂽ���̃X�e�[�g���擾
			}

		}

	}
}