#pragma once
#include "Maruyama/Player/Object/PlayerObject.h"

namespace basecross
{
namespace StageObject
{
	class SelfPlayerObject : public PlayerObject
	{
	public:
		SelfPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		void OnlineSetting(int gameNumber, int pleyerNumber) override;
	};
}
}