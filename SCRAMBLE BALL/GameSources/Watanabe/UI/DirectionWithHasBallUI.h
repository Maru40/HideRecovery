#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"

namespace basecross {
	class DirectionWithHasBallUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_cursor;
		shared_ptr<Camera> m_camera;
		shared_ptr<SimpleSprite> CreateSprite(const wstring& spriteKey);
	public:
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
