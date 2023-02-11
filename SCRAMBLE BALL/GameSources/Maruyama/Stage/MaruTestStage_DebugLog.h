#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross
{

	class MaruTestStage_DebugLog :public GameStageBase {
		void CreateViewLight();

	public:
		MaruTestStage_DebugLog() :GameStageBase() {}

		void OnCreate() override;
		void OnUpdate() override;

		void CreateTestMap();
	};

}
