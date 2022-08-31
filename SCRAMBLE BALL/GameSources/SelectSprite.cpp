/*!
@file SelectSprite.cpp
@brief SelectSpriteのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAction.h"
#include "SelectSprite.h"

namespace basecross {

	SelectSprite::SelectSprite(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void SelectSprite::AddSelectAction(const bool isSelect, const std::function<void()>& function) {
		isSelect ? m_selectAction.AddFunction(function) : m_unSelectAction.AddFunction(function);
	}

	void SelectSprite::SelectInvoke(const bool isSelect) {
		isSelect ? m_selectAction.Invoke() : m_unSelectAction.Invoke();
	}

	void SelectSprite::AddDecisionAction(const std::function<void()>& function) {
		m_decisionAction.AddFunction(function);
	}

	void SelectSprite::DecisionInvoke() {
		m_decisionAction.Invoke();
	}

}