/*!
@file I_Selecter.cpp
@brief I_Selecter�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Selecter.h"

#include "I_PriorityController.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

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
			/// �\�[�g�p�̊��S�B���֐�
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

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X
			//--------------------------------------------------------------------------------------

			SelecterBase::SelecterBase() :
				SelecterBase(SelectType::Priority)
			{}

			SelecterBase::SelecterBase(const SelectType selectType) :
				m_selectType(selectType)
			{}

			void SelecterBase::OnStart() {

			}

			bool SelecterBase::OnUpdate() {
				return true;
			}

			void SelecterBase::OnExit() {

			}
				
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

			std::shared_ptr<I_Node> SelecterBase::GetCurrentNode() const {
				auto currentTransitionData = m_currentTransitionData.lock();
				if (!currentTransitionData) {
					return nullptr;
				}

				return currentTransitionData->node.lock();
			}

			

		}
	}
}