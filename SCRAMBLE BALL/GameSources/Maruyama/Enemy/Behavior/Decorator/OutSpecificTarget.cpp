/*!
@file OutSpecificTarget.cpp
@brief OutSpecificTarget�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"
#include "OutSpecificTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/TargetManager.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				OutSpecificTarget::OutSpecificTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const std::shared_ptr<GameObject>& target
				):
					DecoratorBase(owner),
					m_target(target)
				{
					auto object = owner->GetGameObject();

					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				bool OutSpecificTarget::CanTransition() const {
					//�R���|�[�l���gnull�`�F�b�N
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;
					}

					//�^�[�Q�b�g�Ǘ��Ǝw�肳�ꂽ�^�[�Q�b�g�������Ȃ�A�J�ڂł��Ȃ�
					if (targetManager->GetTarget() == GetTarget()) {
						return false;
					}

					return true;	//����ȊO�̃^�[�Q�b�g�Ȃ�J�ډ\�B
				}

				bool OutSpecificTarget::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}