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
#include "Maruyama/Utility/Utility.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �\�[�g�p�̊��S�B���֐�
			//--------------------------------------------------------------------------------------

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
			/// �r�w�C�r�A�Z���N�^�[�{��
			//--------------------------------------------------------------------------------------

			Selecter::Selecter():
				Selecter(SelectType::Priority)
			{}

			Selecter::Selecter(const SelectType selectType) :
				m_selectType(selectType)
			{}

			void Selecter::OnStart() {

			}

			bool Selecter::OnUpdate() {
				return false;
			}

			void Selecter::OnExit() {
				//�J�ڐ�̃G�b�W�����̏�Ԃɖ߂��B
				for (auto& edge : m_transitionEdges) {
					edge.lock()->GetToNode()->SetState(BehaviorState::Inactive);
				}

				SetCurrentNode(nullptr);	//�m�[�h��nullptr�ɕύX
			}

			std::shared_ptr<I_Node> Selecter::SearchCurrentNode() const {
				switch (m_selectType)
				{
				case maru::Behavior::SelectType::Priority:
					return SearchFirstPriorityNode();
				case maru::Behavior::SelectType::Random:
					return SearchRandomNode();
				case maru::Behavior::SelectType::Sequence:
					return SearchSequenceNode();
				}

				return nullptr;
			}

			std::shared_ptr<I_Node> Selecter::SearchFirstPriorityNode() const {
				//���݂̃X�e�[�g��Running�Ȃ�A��x���������Ă��邽�߁A�I���B
				if (IsState(BehaviorState::Running)) {
					return nullptr;
				}

				//�J�ڐ�m�[�h����Ȃ�nullptr
				if (IsEmptyTransitionNodes()) {
					return nullptr;
				}

				auto transitionEdges = m_transitionEdges;		//�����o���\�[�g�����const�ɂł��Ȃ����߁A�ꎞ�ϐ���

				//�G�b�W�̗D��x�v�Z���ɂ���B
				for (auto& weakEdge : transitionEdges) {
					if (weakEdge.expired()) {	//���݂��Ȃ��Ȃ�
						continue;
					}

					weakEdge.lock()->CalculatePriority();
				}

				//�����\�[�g
				std::sort(transitionEdges.begin(), transitionEdges.end(), &SortEdges);	

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

			std::shared_ptr<I_Node> Selecter::SearchSequenceNode() const {
				//�ς܂ꂽ�ォ�珇�ɑJ�ڂł���^�X�N�ɑJ�ځB
				for (auto& edge : m_transitionEdges) {
					auto toNode = edge.lock()->GetToNode();
					if (toNode->CanTransition()) {
						return toNode;
					}
				}

				return nullptr;
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

			void Selecter::SetCurrentNode(const std::shared_ptr<I_Node>& node) noexcept {
				m_currentNode = node;
			}

			std::shared_ptr<I_Node> Selecter::GetCurrentNode() const noexcept {
				return m_currentNode.lock();
			}

		}
	}
}