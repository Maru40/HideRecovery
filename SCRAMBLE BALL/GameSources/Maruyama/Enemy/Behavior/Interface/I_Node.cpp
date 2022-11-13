/*!
@file I_Node.cpp
@brief I_Node�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

#include "I_Decorator.h"

namespace basecross {
	namespace maru {

		namespace Behavior {


			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�^�X�N�̊��N���X
			//--------------------------------------------------------------------------------------

			void NodeBase::OnDecoratorStart() {
				for (auto& decorator : m_decorators) {
					decorator->OnStart();
				}
			}

			void NodeBase::OnDecoratorExit() {
				for (auto& decorator : m_decorators) {
					decorator->OnExit();
				}
			}

			bool NodeBase::CanTransition() const {
				if (!IsActive()) {	//��A�N�e�B�u�Ȃ�J�ڂł��Ȃ����߁Afalse
					return false;
				}

				//������ԂȂ�J�ڂ��Ȃ��B
				if (IsState(BehaviorState::Completed)) {
					return false;
				}

				//�f�R���[�^���Ȃ��Ȃ�A���true
				if (IsDecoratorEmpty()) {	
					return true;
				}

				//��ł��J�ڂł��Ȃ��Ȃ�false
				for (const auto& decorator : m_decorators) {
					decorator->ReserveCanTransition();
					if (!decorator->CanTransition()) {
						return false;
					}
				}

				return true;	//�S�Ẵf�R���[�^��true�Ȃ�J�ڂł���B
			}

			bool NodeBase::CanUpdate() const {
				if (IsDecoratorEmpty()) {
					return true;
				}

				//��ł��X�V�ł��Ȃ��Ȃ�false
				for (const auto& decorator : m_decorators) {
					if (!decorator->CanUpdate()) {
						return false;
					}
				}

				return true;	//�S�Ẵf�R���[�^��true�Ȃ�X�V�ł���B
			}

			void NodeBase::AddDecorator(const std::shared_ptr<I_Decorator>& decorator) {
				m_decorators.push_back(decorator);
			}

			std::vector<std::shared_ptr<I_Decorator>> NodeBase::GetDecorators() const {
				return m_decorators;
			}

			bool NodeBase::IsDecoratorEmpty() const {
				return static_cast<int>(m_decorators.size()) == 0;
			}

		}
	}
}