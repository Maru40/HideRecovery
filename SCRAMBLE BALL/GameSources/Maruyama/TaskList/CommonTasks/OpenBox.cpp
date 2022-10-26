
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMoveのクラス実体
担当：丸山裕喜
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
			Open();				//ボックスを開く

			ChangeAnimation();	//アニメーションの切り替え
		}

		bool OpenBox::OnUpdate() {
			//将来的にアニメーションが終了するまでを条件とする。

			return true;
		}

		void OpenBox::OnExit() {

		}

		void OpenBox::Open() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : 必要コンポーネントが存在しません。");
				return;
			}

			auto target = targetManager->GetTarget();

			auto hidePlace = target->GetComponent<HidePlace>(false);
			if (hidePlace) {
				hidePlace->Open();
				Debug::GetInstance()->Log(L"Open");
			}
			else {
				Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : ターゲットがHidePlaceでありません。");
			}

			//if (!m_accessHidePlace.expired()) {
			//	m_accessHidePlace.lock()->Access();
			//	Debug::GetInstance()->Log(L"Open");
			//}
			//else {
			//	Debug::GetInstance()->Log(L"TaskListNode::OpenBox::Open() : 必要コンポーネントが存在しません。");
			//}
		}

		void OpenBox::ChangeAnimation() {

		}

	}
}