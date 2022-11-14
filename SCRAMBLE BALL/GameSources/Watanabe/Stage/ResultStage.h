#pragma once
#include "stdafx.h"
#include "GameStageBase.h"
#include "../TimeLine/CameraTimeLine.h"

namespace basecross {
	namespace team {
		enum class TeamType;
	}

	class ResultStage :public GameStageBase {
		weak_ptr<timeline::CameraTimeLine> m_timeLine;
		void CreateViewLight();

		void CreateUIAnimation(const shared_ptr<UIObjectBase>& uiObject, const Vec2& offset);
	public:
		ResultStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;

	private:
		void CreatePlayers(const team::TeamType winerType);
	};
}
