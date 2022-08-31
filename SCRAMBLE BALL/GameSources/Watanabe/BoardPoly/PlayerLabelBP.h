#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "../Manager/SpriteDataManager.h"

namespace basecross {
	class PlayerLabelBP :public StageObjectBase {
		wstring m_teamStr;
		int m_number;
		shared_ptr<StageObjectBase> CreateBoard(const sdm::SpriteData& spriteData);
	public:
		PlayerLabelBP(const shared_ptr<Stage>& stage,
			const wstring& teamStr, int number);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
