#pragma once
#include "stdafx.h"

namespace basecross {
	class TestComponent :public Component {
	private:
		using CreateFunc = function<void(void)>;
		using UpdateFunc = function<void(float)>;
		CreateFunc m_fOnCreate;
		UpdateFunc m_fOnUpdate;
		float m_delta = 0;
	public:
		TestComponent(const shared_ptr<GameObject>& owner)
			:Component(owner)
		{}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void SetOnCreateFunction(CreateFunc func);
		void SetOnUpdateFunction(UpdateFunc func);

		void ResetOnCreateFunction();
		void ResetOnUpdateFunction();
	};
}
