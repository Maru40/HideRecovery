#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross {
	class ResultStage :public GameStageBase {
		void CreateViewLight();
	public:
		ResultStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;
	};
}
