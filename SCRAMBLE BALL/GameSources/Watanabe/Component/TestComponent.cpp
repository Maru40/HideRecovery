#include "stdafx.h"
#include "TestComponent.h"

namespace basecross {
	void TestComponent::OnCreate() {
		if (m_fOnCreate)
			m_fOnCreate();
	}
	void TestComponent::OnUpdate() {
		if (m_fOnUpdate)
			m_fOnUpdate(m_delta);
		m_delta += App::GetApp()->GetElapsedTime();
	}

	void TestComponent::SetOnCreateFunction(CreateFunc func) { m_fOnCreate = func; }
	void TestComponent::SetOnUpdateFunction(UpdateFunc func) { m_fOnUpdate = func; }

	void TestComponent::ResetOnCreateFunction() { m_fOnCreate = nullptr; }
	void TestComponent::ResetOnUpdateFunction() { m_fOnUpdate = nullptr; }
}