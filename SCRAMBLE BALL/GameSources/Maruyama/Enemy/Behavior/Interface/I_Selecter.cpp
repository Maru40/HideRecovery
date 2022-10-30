/*!
@file I_Selecter.cpp
@brief I_Selecter�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Selecter.h"

#include "I_Edge.h"
#include "I_PriorityController.h"

#include "Maruyama/TaskList/TaskList.h"

#include "Maruyama/Utility/Random.h"

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

			/// <summary>
			/// �J�ڐ��D�揇�ʏ��Ƀ\�[�g�������
			/// </summary>
			/// <param name="right">��r�Ώۂ̃f�[�^</param>
			/// <param name="left">��r�Ώۂ̃f�[�^</param>
			/// <returns>left���������Ȃ�true</returns>
			bool SortEdges(
				const std::weak_ptr<I_Edge>& left,
				const std::weak_ptr<I_Edge>& right
			) {
				return left.lock()->GetPriority() < right.lock()->GetPriority();	//�D�揇�ʂ��Ⴂ���Ƀ\�[�g
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

			void SelecterBase::ChangeCurrentNode(const std::shared_ptr<I_Node>& node) {
				for (auto data : m_transitionDatas) {
					if (data->node.lock() == node) {
						m_currentTransitionData = data;
						break;
					}
				}
			}
			

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̉��ǌ^(�J���r���̂��߁A�g�p�֎~)
			//--------------------------------------------------------------------------------------

			Selecter::Selecter():
				Selecter(SelectType::Priority)
			{}

			Selecter::Selecter(const SelectType selectType) :
				m_selectType(selectType)
			{ }

			void Selecter::OnStart() {
				SetState(BehaviorState::Running);
			}

			bool Selecter::OnUpdate() {
				//���ݎg�p���Ă��Ȃ�

				if (!HasCurrentNode()) {
					return true;
				}

				auto currentNode = GetCurrentNode();

				//�J�����g�m�[�h��������ԂȂ珈�����I������B
				if (currentNode->IsState(BehaviorState::Completed)) {
					return true;
				}

				return currentNode->OnUpdate();	//�J�����g�m�[�h���A�b�v�f�[�g
			}

			void Selecter::OnExit() {
				//�J�ڐ�̃G�b�W�����̏�Ԃɖ߂��B
				for (auto& edge : m_transitionEdges) {
					edge.lock()->GetToNode()->SetState(BehaviorState::Inactive);
				}

				SetState(BehaviorState::Completed);
			}


			std::shared_ptr<I_Node> Selecter::SearchCurrentNode() const {
				switch (m_selectType)
				{
				case maru::Behavior::SelectType::Priority:
					return SearchFirstPriorityNode();
				case maru::Behavior::SelectType::Random:
					return SearchRandomNode();
				}

				return nullptr;
			}

			std::shared_ptr<I_Node> Selecter::SearchFirstPriorityNode() const {
				//�J�ڐ�m�[�h����Ȃ�nullptr
				if (IsEmptyTransitionNodes()) {
					return nullptr;
				}

				auto transitionEdges = m_transitionEdges;		//�����o���\�[�g�����const�ɂł��Ȃ����߁A�ꎞ�ϐ���
				std::sort(transitionEdges.begin(), transitionEdges.end(), &SortEdges);	//�����\�[�g

				//���בւ����m�[�h���J�ڂł��邩�ǂ����𔻒f����B
				for (const auto& edge : transitionEdges) {
					if (edge.lock()->GetToNode()->CanTransition()) {	//�J�ڂł���Ȃ�A���̃m�[�h��Ԃ��B
						return edge.lock()->GetToNode();
					}
				}

				return nullptr;
			}

			std::shared_ptr<I_Node> Selecter::SearchRandomNode() const {
				std::vector<std::shared_ptr<I_Edge>> transitionEdges;
				for (auto edge : m_transitionEdges) {
					if (edge.lock()->GetToNode()->CanTransition()) {
						transitionEdges.push_back(edge.lock());
					}
				}

				if (transitionEdges.empty()) {	//�J�ڐ悪���݂��Ȃ��Ȃ�nullptr��Ԃ��B
					return nullptr;
				}

				auto randomEdge = maru::MyRandom::RandomArray(transitionEdges);
				return randomEdge->GetToNode();
			}

			void Selecter::ChangeCurrentNode(const std::shared_ptr<I_Node>& node) {
				//���݂̃m�[�h�̏I������������B
				if (auto currentNode = GetCurrentNode()) {
					currentNode->OnExit();
				}

				m_currentNode = node;

				//�ύX����m�[�h��null�łȂ�������A�J�n����������B
				if (node) {
					node->OnStart();
				}
			}

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			void Selecter::SetFromEdge(const std::shared_ptr<I_Edge>& fromEdge) noexcept {
				m_fromEdge = fromEdge;
			}

			std::shared_ptr<I_Edge> Selecter::GetFromEdge() const noexcept {
				return m_fromEdge.lock();
			}

			void Selecter::AddTransitionEdge(const std::shared_ptr<I_Edge>& edge) {
				m_transitionEdges.push_back(edge);
			}

			bool Selecter::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionEdges.size()) == 0;
			}

			std::shared_ptr<I_Node> Selecter::GetCurrentNode() const noexcept {
				return m_currentNode.lock();
			}

			std::shared_ptr<I_Node> Selecter::CheckCurrentNode() const {
				if (!HasCurrentNode()) {
					return nullptr;
				}

				auto currentNode = GetCurrentNode();
				if (currentNode->IsState(BehaviorState::Completed)) {	//������ԂȂ珈�������Ȃ��B
					return nullptr;
				}

				return GetCurrentNode();
			}

		}
	}
}