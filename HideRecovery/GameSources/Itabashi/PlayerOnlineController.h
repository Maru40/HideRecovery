#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class RotationController;

namespace Operator
{
	class ObjectMover;
}

namespace Online
{

	class PlayerOnlineController : public OnlineComponent
	{
		std::weak_ptr<Operator::ObjectMover> m_objectMover;
		std::weak_ptr<RotationController> m_rotationController;

		int m_playerNumber = 0;

	public:
		static constexpr std::uint8_t EVENT_CODE = 2;

		PlayerOnlineController(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnDraw() override {}

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		void SetPlayerNumber(int playerNumber) { m_playerNumber = playerNumber; }
		int GetPlayerNumber() const { return m_playerNumber; }
	};
}
}