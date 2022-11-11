
/*!
@file AIVirtualController.h
@brief AIVirtualController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class AIVirtualController : public Component
	{
		Vec2 m_inputDirection;			//���͕���
		Vec2 m_targetInputDirection;	//�ڕW�̓��͕���

	public:
		AIVirtualController(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		void SetInputDirection(const Vec2& direction);

		Vec2 GetInputDirection() const noexcept { return m_inputDirection; }
	};

}