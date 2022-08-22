/*!
@file   OwnAreaObject.h
@brief  自陣エリアクラス
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "PlayerSpawnPointObject.h"

namespace basecross {
	class OwnAreaObject : public StageObjectBase {
		// エリアの半径
		float m_areaRadius;
		// チーム
		Team m_team;
		// メッシュデータ
		shared_ptr<MeshResource> m_meshRes;
	public:
		OwnAreaObject(const shared_ptr<Stage>& stage);
		OwnAreaObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
