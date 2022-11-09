
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMoveのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "OpenBox.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "VelocityManager.h"

#include "Maruyama/StageObject/HidePlace.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Maruyama/Player/Component/HidePlaceOpener.h"

#include "Itabashi/OnlinePlayerSynchronizer.h"

namespace basecross {

	namespace TaskListNode {

		OpenBox::OpenBox(const std::shared_ptr<GameObject>& owner) :
			TaskNodeBase(owner)
		{
			m_targetManager = owner->GetComponent<TargetManager>(false);
			m_animator = owner->GetComponent<PlayerAnimator>(false);
			m_hidePlaceOpener = owner->GetComponent<HidePlaceOpener>(false);
			m_velocityManager = owner->GetComponent<VelocityManager>(false);
			m_onlineSynchronizer = owner->GetComponent<OnlinePlayerSynchronizer>(false);
		}

		void OpenBox::OnStart() {
			Open();				//ボックスを開く

			ChangeAnimation();	//アニメーションの切り替え

			if (!m_velocityManager.expired()) {
				m_velocityManager.lock()->ResetAll();
			}
		}

		bool OpenBox::OnUpdate() {
			//将来的にアニメーションが終了するまでを条件とする。

			return true;
		}

		void OpenBox::OnExit() {

		}

		void OpenBox::Open() {
			m_onlineSynchronizer.lock()->OpenHidePlace();

			//ターゲットの有無を確認する。
			//auto targetManager = m_targetManager.lock();
			//if (!targetManager || !targetManager->HasTarget()) {
			//	Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : 必要コンポーネントが存在しません。");
			//	return;
			//}

			//auto target = targetManager->GetTarget();

			////ターゲットがHidePlaceならOpenする。
			//auto hidePlace = target->GetComponent<HidePlace>(false);
			//if (hidePlace) {
			//	hidePlace->Open();
			//	Debug::GetInstance()->Log(L"Open");
			//}
			//else {
			//	Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : ターゲットがHidePlaceでありません。");
			//}
		}

		void OpenBox::ChangeAnimation() {
			if (m_animator.lock() == nullptr) {
				return;
			}

			m_animator.lock()->ChangePlayerAnimation(PlayerAnimationState::State::PutItem_Floor);
		}

	}
}