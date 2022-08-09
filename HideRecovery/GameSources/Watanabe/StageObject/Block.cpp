/*!
@file   Block.cpp
@brief  ステージを構成するブロッククラス実体
*/

#include "stdafx.h"
#include "Block.h"

namespace basecross {
	Block::Block(const std::shared_ptr<Stage> stage, const BlockType blockType)
		:GameObject(stage), m_blockType(blockType)
	{}

	Block::Block(const std::shared_ptr<Stage> stage, const wstring line)
		: GameObject(stage)
	{
		//vector<wstring> tokens = DataExtracter::DelimitData(line);
	}

	void Block::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetTextureResource(L"Wall01_TX");

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
		AddTag(L"T_Obstacle");
		AddTag(L"Wall");
	}

	void Block::OnUpdate() {
	}
}