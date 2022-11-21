
/*!
@file AIVirtualController.h
@brief AIVirtualController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class PlayerStatus;
	class OnlinePlayerSynchronizer;

	class AIVirtualController : public Component
	{
		Vec2 m_inputDirection;			//���͕���

		std::weak_ptr<PlayerStatus> m_status;							//�X�e�[�^�X
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//�I�����C��

	public:
		AIVirtualController(const std::shared_ptr<GameObject>& objPtr);

		void OnStart() override;

		void SetInputDirection(const Vec2& direction);

		Vec2 GetInputDirection() const noexcept { return m_inputDirection; }

		void OnCollisionEnter(std::shared_ptr<GameObject>& other);
	};

}