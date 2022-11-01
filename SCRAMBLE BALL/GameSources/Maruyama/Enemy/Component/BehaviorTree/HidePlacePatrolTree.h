/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/SubBehaviorTreeBase.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				enum class HidePlacePatrolTree_BehaviorType {
					FirstSelecter,		//����m�[�h

					PatrolTask,			//�p�g���[���^�X�N
				};

				class HidePlacePatrolTree : public SubBehaviorTreeBase<HidePlacePatrolTree_BehaviorType>
				{
				public:
					using BehaviorType = HidePlacePatrolTree_BehaviorType;

				public:
					HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr);

					virtual ~HidePlacePatrolTree() = default;

				protected:

					void CreateNode() override;
					void CreateEdge() override;
					void CreateDecorator() override;

				public:

				};

			}
		}
	}
}