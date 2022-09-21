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

		}
	}
}