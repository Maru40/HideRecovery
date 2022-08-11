#include "stdafx.h"
#include "TestComponent.h"

namespace basecross {
	void TestComponent::OnCreate() {
		if (m_fOnCreate)
			m_fOnCreate();
	}
	void TestComponent::OnUpdate() {
		if (m_fOnUpdate)
			m_fOnUpdate();
	}

	void TestComponent::SetOnCreateFunction(Func func) { m_fOnCreate = func; }
	void TestComponent::SetOnUpdateFunction(Func func) { m_fOnUpdate = func; }

	void TestComponent::ResetOnCreateFunction() { m_fOnCreate = nullptr; }
	void TestComponent::ResetOnUpdateFunction() { m_fOnUpdate = nullptr; }
}