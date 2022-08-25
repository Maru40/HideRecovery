#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross {
	class TitleStage :public GameStageBase {
		void CreateViewLight();
	public:
		TitleStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;
	};
}
