/*!
@file HeightDestroy.cpp
@brief HeightDestroy�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "HeightDestroy.h"

#include "Maruyama/Utility/MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���̍����Ŏ��S������o���R���|�[�l���g�̃p�����[�^
	//--------------------------------------------------------------------------------------

	HeightDestroy_Parametor::HeightDestroy_Parametor():
		height(-5.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// ���̍����Ŏ��S������o���R���|�[�l���g�̖{��
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
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void HeightDestroy::AddDestroyAction(const std::function<void()>& func) {
		m_action->AddFunction(func);
	}

	bool HeightDestroy::IsDestroyHeight() const {
		return transform->GetPosition().y < m_param.height;
	}
}