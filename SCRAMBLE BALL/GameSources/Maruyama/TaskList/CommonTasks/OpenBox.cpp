
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMove�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OpenBox.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Player/Component/AccessHidePlace.h"

#include "Maruyama/StageObject/HidePlace.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace TaskListNode {

		OpenBox::OpenBox(const std::shared_ptr<GameObject>& owner) :
			TaskNodeBase(owner)
		{
			m_targetManager = owner->GetComponent<TargetManager>(false);
			m_accessHidePlace = owner->GetComponent<AccessHidePlace>(false);
		}

		void OpenBox::OnStart() {
			Open();				//�{�b�N�X���J��

			ChangeAnimation();	//�A�j���[�V�����̐؂�ւ�
		}

		bool OpenBox::OnUpdate() {
			//�����I�ɃA�j���[�V�������I������܂ł������Ƃ���B

			return true;
		}

		void OpenBox::OnExit() {

		}

		void OpenBox::Open() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : �K�v�R���|�[�l���g�����݂��܂���B");
				return;
			}

			auto target = targetManager->GetTarget();

			auto hidePlace = target->GetComponent<HidePlace>(false);
			if (hidePlace) {
				hidePlace->Open();
				Debug::GetInstance()->Log(L"Open");
			}
			else {
				Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : �^�[�Q�b�g��HidePlace�ł���܂���B");
			}

			//if (!m_accessHidePlace.expired()) {
			//	m_accessHidePlace.lock()->Access();
			//	Debug::GetInstance()->Log(L"Open");
			//}
			//else {
			//	Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : �K�v�R���|�[�l���g�����݂��܂���B");
			//}
		}

		void OpenBox::ChangeAnimation() {

		}

	}
}