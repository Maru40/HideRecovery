
/*!
@file AIVirtualController.h
@brief AIVirtualControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class PlayerStatus;
	class OnlinePlayerSynchronizer;

	class AIVirtualController : public Component
	{
		Vec2 m_inputDirection;			//入力方向

		std::weak_ptr<PlayerStatus> m_status;							//ステータス
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//オンライン

	public:
		AIVirtualController(const std::shared_ptr<GameObject>& objPtr);

		void OnStart() override;

		void SetInputDirection(const Vec2& direction);

		Vec2 GetInputDirection() const noexcept { return m_inputDirection; }

		void OnCollisionEnter(std::shared_ptr<GameObject>& other);
	};

}