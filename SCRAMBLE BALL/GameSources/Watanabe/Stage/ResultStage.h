#pragma once
#include "stdafx.h"
#include "GameStageBase.h"
#include "../UI/UIObjectBase.h"

namespace basecross {
	namespace team {
		enum class TeamType;
	}

	class ResultStage :public GameStageBase {
		// trueなら遷移可能
		bool m_isTransitionable = false;

		void CreateViewLight();

		void CreateUIAnimation(const wstring& clipName, const shared_ptr<UIObjectBase>& uiObject, const Vec2& offset);
	public:
		ResultStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;

	private:
		void CreatePlayers(const team::TeamType winerType);
	};
}
