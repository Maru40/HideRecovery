#pragma once
#include "stdafx.h"

namespace basecross {
	class TestComponent :public Component {
	private:
		using Func = function<void(void)>;
		Func m_fOnCreate;
		Func m_fOnUpdate;
	public:
		TestComponent(const shared_ptr<GameObject>& owner)
			:Component(owner)
		{}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void SetOnCreateFunction(Func func);
		void SetOnUpdateFunction(Func func);

		void ResetOnCreateFunction();
		void ResetOnUpdateFunction();
	};
}
