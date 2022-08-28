﻿#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"
#include "../StageObject/PlayerSpawnPointObject.h"

namespace basecross {
	class WinOrLoseUI :public UIObjectBase {
		vector<shared_ptr<SimpleSprite>> m_sprites;
	public:
		WinOrLoseUI(const shared_ptr<Stage>& stage);
		WinOrLoseUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void SetTeam(team::TeamType team);
	};
}
