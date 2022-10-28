/*!
@file I_Selecter.cpp
@brief I_Selecter�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Priority.h"

#include "../Interface/I_Node.h"
#include "../Interface/I_Selecter.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace SelecterTask {

				Priority::Priority(const std::shared_ptr<Selecter>& selecter):
					TaskNodeBase(selecter)
				{}

				void Priority::OnStart() {
					auto selecter = GetOwner();

					//��ԗD��x�̍����m�[�h�̎擾
					//auto node = selecter->SearchFirstPriorityNode();
					//
					//selecter->ChangeCurrentNode(node);	//�Z���N�^�[�̃J�����g�m�[�h�ɐݒ�
					//m_currentNode = node;				//�������g�̃J�����g�m�[�h�ɐݒ�
				}

				bool Priority::OnUpdate() {
					if (HasCurrentNode()) {
						return m_currentNode.lock()->OnUpdate();
					}

					return true;
				}

				void Priority::OnExit() {
					m_currentNode.reset();
				}

				bool Priority::HasCurrentNode() const {
					return !m_currentNode.expired();
				}

			}
		}
	}
}