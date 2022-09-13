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

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�^�X�N�̊��N���X
			//--------------------------------------------------------------------------------------

			bool NodeBase::CanTransition() const {
				if (IsDecoratorEmpty()) {	//�f�R���[�^���Ȃ��Ȃ�A���true
					return true;
				}

				//��ł��J�ڂł��Ȃ��Ȃ�false
				for (const auto& decorator : m_decorators) {
					if (!decorator->CanTransition()) {
						return false;
					}
				}

				return true;	//�S�Ẵf�R���[�^��true�Ȃ�J�ڂł���B
			}

			void NodeBase::AddDecorator(const std::shared_ptr<I_Decorator>& decorator) {
				m_decorators.push_back(decorator);
			}

			bool NodeBase::IsDecoratorEmpty() const {
				return static_cast<int>(m_decorators.size()) == 0;
			}

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �J�ڐ�m�[�h��D�揇�ʏ��Ƀ\�[�g�������
			/// </summary>
			/// <param name="leftData"></param>
			/// <param name="rightData"></param>
			/// <returns></returns>
			bool SortTransitionNodeData(
				const std::shared_ptr<I_Selecter::TransitionNodeData> leftData, 
				const std::shared_ptr<I_Selecter::TransitionNodeData> rightData
			) {
				//return true;
				return leftData->priorityController->GetPriority() < rightData->priorityController->GetPriority();
			}

			SelecterBase::SelecterBase(const std::shared_ptr<I_Node>& fromNode) :
				m_fromNode(fromNode)
			{}

			std::shared_ptr<I_Node> SelecterBase::GetFirstPriorityNode() const {
				if (IsEmptyTransitionNodes()) {	//�J�ڐ�m�[�h����Ȃ�nullptr
					return nullptr;
				}

				for (auto data : m_transitionNodes) {
					data;
					data->priorityController;
					auto isMax = data->priorityController->GetPriority() < data->priorityController->GetPriority();

					SortTransitionNodeData(data, data);
				}

				//std::sort(m_transitionNodes.begin(), m_transitionNodes.end(), &SortTransitionNodeData);

				//return nullptr;
				return m_transitionNodes[0]->node.lock();	//��ԗD��x�������m�[�h��Ԃ��B
			}

			bool SelecterBase::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionNodes.size()) == 0;
			}

		}

	}
}