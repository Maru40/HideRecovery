#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class GoalBP :public StageObjectBase {
		shared_ptr<StageObjectBase> CreateBoard(const wstring& spriteDataKey);
	public:
		GoalBP(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
