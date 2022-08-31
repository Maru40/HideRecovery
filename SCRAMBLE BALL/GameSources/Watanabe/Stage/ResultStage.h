#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross {
	namespace team {
		enum class TeamType;
	}

	class ResultStage :public GameStageBase {
		void CreateViewLight();
	public:
		ResultStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;

	private:
		void CreatePlayers(const team::TeamType winerType);
	};
}
