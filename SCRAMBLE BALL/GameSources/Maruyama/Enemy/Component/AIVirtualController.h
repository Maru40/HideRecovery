
/*!
@file AIVirtualController.h
@brief AIVirtualControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class AIVirtualController : public Component
	{
		Vec2 m_inputDirection;			//入力方向
		Vec2 m_targetInputDirection;	//目標の入力方向

	public:
		AIVirtualController(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void SetInputDirection(const Vec2& direction);

		Vec2 GetInputDirection() const noexcept { return m_inputDirection; }
	};

}