/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/SubBehaviorTree/SubBehaviorTreeBase.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace Task {
				struct NearAstarMove_Parametor;
				struct NearSeekMove_Parametor;
			}

			namespace SubBehavior {

				enum class HidePlacePatrolTree_BehaviorType {
					FirstSelecter,			//����Z���N�^�[

					ToGoalRunTask,			//�S�[���܂ōs���^�X�N�B
					ToMoveHasBallEnemyTask,	//�{�[���������Ă���G�܂ōs���^�X�N�B
					RelifHasBallMemberTask,	//�{�[���������Ă��郁���o�[�����^�X�N�B
					ToBallRunTask,			//�{�[���܂ŋ삯����^�X�N�B
					PatrolTask,				//�p�g���[���^�X�N�B
					RandomMemberRelif,		//�����_���ȃ����o�[�����B
				};

				//--------------------------------------------------------------------------------------
				/// �B��ꏊ��T���p�g���[���r�w�C�r�A�c���[�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct HidePlacePatrolTree_Parametor {

					HidePlacePatrolTree_Parametor();

					~HidePlacePatrolTree_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// �B��ꏊ��T���p�g���[���r�w�C�r�A�c���[
				//--------------------------------------------------------------------------------------
				class HidePlacePatrolTree : public SubBehaviorTreeBase<HidePlacePatrolTree_BehaviorType>
				{
				public:
					using Parametor = HidePlacePatrolTree_Parametor;
					using BehaviorType = HidePlacePatrolTree_BehaviorType;

				private:
					Parametor m_param;

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