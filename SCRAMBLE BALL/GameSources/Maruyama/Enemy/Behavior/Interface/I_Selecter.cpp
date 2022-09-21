/*!
@file I_Selecter.cpp
@brief I_Selecter�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Selecter.h"

#include "I_PriorityController.h"

namespace basecross {
	namespace maru {

		namespace Behavior {


			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̑J�ڐ�m�[�h�f�[�^
			//--------------------------------------------------------------------------------------

			Selecter_TransitionNodeData::Selecter_TransitionNodeData(
				const std::shared_ptr<I_PriorityController>& priorityController,
				const std::shared_ptr<I_Node>& node
			) :
				priorityController(priorityController),
				node(node)
			{}

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �J�ڐ�m�[�h��D�揇�ʏ��Ƀ\�[�g�������
			/// </summary>
			/// <param name="leftData">��r�Ώۂ̃f�[�^</param>
			/// <param name="rightData">��r�Ώۂ̃f�[�^</param>
			/// <returns>leftData���������Ȃ�true</returns>
			bool SortTransitionNodeData(
				const std::shared_ptr<I_Selecter::TransitionNodeData> leftData,
				const std::shared_ptr<I_Selecter::TransitionNodeData> rightData
			) {
				return leftData->priorityController->GetPriority() < rightData->priorityController->GetPriority();
			}

			SelecterBase::SelecterBase(const std::shared_ptr<I_Node>& fromNode) :
				m_fromNode(fromNode)
			{}

			std::shared_ptr<I_Node> SelecterBase::GetFirstPriorityNode() const {
				if (IsEmptyTransitionNodes()) {	//�J�ڐ�m�[�h����Ȃ�nullptr
					return nullptr;
				}

				auto transitionDatas = m_transitionDatas;	//�����o���\�[�g�����const�ɂł��Ȃ����߁A�ꎞ�ϐ���
				std::sort(transitionDatas.begin(), transitionDatas.end(), &SortTransitionNodeData);	//�����\�[�g

				return transitionDatas[0]->node.lock();	//��ԗD��x�������m�[�h��Ԃ��B
			}

			bool SelecterBase::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionDatas.size()) == 0;
			}

		}
	}
}