/*!
@file   Block.h
@brief  ステージを構成するブロッククラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Block :public GameObject {
	public:
		enum class BlockType {
			Wall,	// 壁
			Floor	// 床
		};
	private:
		BlockType m_blockType;
	public:
		Block(const std::shared_ptr<Stage> stage, const BlockType blockType);
		Block(const std::shared_ptr<Stage> stage, const wstring line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
