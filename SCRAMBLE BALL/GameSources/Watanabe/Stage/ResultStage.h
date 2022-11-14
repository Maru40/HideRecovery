#pragma once
#include "stdafx.h"
#include "GameStageBase.h"
#include "../TimeLine/TimeLine.h"

namespace basecross {
	namespace team {
		enum class TeamType;
	}

	class ResultStage :public GameStageBase {
		weak_ptr<TimeLine> m_timeLine;
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
