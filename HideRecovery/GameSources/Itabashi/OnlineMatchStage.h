#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross
{
	class OnlineMatchStage : public GameStageBase
	{
	public:
		void OnCreate() override;

		void OnUpdate() override;
	};
}