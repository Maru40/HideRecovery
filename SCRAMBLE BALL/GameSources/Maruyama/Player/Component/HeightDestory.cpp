/*!
@file HeightDestroy.cpp
@brief HeightDestroyクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HeightDestroy.h"

#include "Maruyama/Utility/MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 一定の高さで死亡判定を出すコンポーネントのパラメータ
	//--------------------------------------------------------------------------------------

	HeightDestroy_Parametor::HeightDestroy_Parametor():
		height(-5.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// 一定の高さで死亡判定を出すコンポーネントの本体
	//--------------------------------------------------------------------------------------

	HeightDestroy::HeightDestroy(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor()),
		m_action(new maru::Action<void()>())
	{}

	void HeightDestroy::OnUpdate() {
		ObserveHeight();
	}

	void HeightDestroy::ObserveHeight() {
		if (IsDestroyHeight()) {
			Invoke();
		}
	}

	void HeightDestroy::Invoke() {
		m_action->Invoke();
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void HeightDestroy::AddDestroyAction(const std::function<void()>& func) {
		m_action->AddFunction(func);
	}

	bool HeightDestroy::IsDestroyHeight() const {
		return transform->GetPosition().y < m_param.height;
	}
}