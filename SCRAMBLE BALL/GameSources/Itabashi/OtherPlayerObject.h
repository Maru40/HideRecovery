#pragma once
#include "Maruyama/Player/Object/PlayerObject.h"

namespace basecross
{
namespace StageObject
{
	class OtherPlayerObject : public PlayerObject
	{
	public:
		OtherPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		virtual void OnlineSetting(int gameNumber, int playerNumber) override;
	};
}
}