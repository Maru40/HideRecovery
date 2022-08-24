#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class Container :public StageObjectBase {
		shared_ptr<GameObject> m_hitTestObject;
	public:
		Container(const shared_ptr<Stage>& stage);
		Container(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
