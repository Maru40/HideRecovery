#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross 
{

	class MaruTestStage :public GameStageBase {
		void CreateViewLight();

	public:
		MaruTestStage() :GameStageBase() {}

		void OnCreate() override;
		void OnUpdate() override;
	};

}
