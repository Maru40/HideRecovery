﻿/*!
@file   Block.cpp
@brief  ステージを構成するブロッククラス実体
*/

#include "stdafx.h"
#include "Block.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	Block::Block(const shared_ptr<Stage>& stage, const BlockType blockType)
		:StageObjectBase(stage, L"Block"), m_blockType(blockType)
	{}

	Block::Block(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"Block")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		int nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);
		wstring blockType = tokens[nextIndex];
		if (blockType == L"Wall") {
			m_blockType = BlockType::Wall;
		}
		else if (blockType == L"Floor") {
			m_blockType = BlockType::Floor;
		}
		else {
			throw BaseException(
				L"BlockTypeが不正な値です。",
				L"BlockType : " + blockType,
				L"Block::Block(const shared_ptr<Stage>& stage, const wstring& line)"
			);
		}
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