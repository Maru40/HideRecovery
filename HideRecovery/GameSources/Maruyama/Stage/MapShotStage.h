#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross {

	class MapShotStage :public GameStageBase 
	{
		void CreateViewLight();
	public:
		MapShotStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;
	};

}