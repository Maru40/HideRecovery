/*!
@file   Block.h
@brief  ステージを構成するブロッククラス
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class Block :public StageObjectBase {
	public:
		enum class BlockType {
			Wall,	// 壁
			Floor	// 床
		};
	private:
		// ブロックの種類
		BlockType m_blockType;
		// メッシュデータ
		shared_ptr<MeshResource> m_meshRes;
		// このオブジェクトがカメラを遮るか
		bool m_isReactCamera;
	public:
		Block(const shared_ptr<Stage>& stage, const BlockType blockType);
		Block(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
