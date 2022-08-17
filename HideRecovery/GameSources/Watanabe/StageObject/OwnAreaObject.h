/*!
@file   OwnAreaObject.h
@brief  自陣エリアクラス
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "PlayerSpawnPoint.h"

namespace basecross {
	class OwnAreaObject : public StageObjectBase {
		// エリアの半径
		float m_areaRadius;
		// チーム
		Team m_team;
	public:
		OwnAreaObject(const shared_ptr<Stage>& stage);
		OwnAreaObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
