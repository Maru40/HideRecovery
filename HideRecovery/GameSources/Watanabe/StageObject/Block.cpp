/*!
@file   Block.cpp
@brief  ステージを構成するブロッククラス実体
*/

#include "stdafx.h"
#include "Block.h"
#include "../Utility/DataExtracter.h"
#include "../Utility/AdvMeshUtil.h"

namespace basecross {
	Block::Block(const shared_ptr<Stage>& stage, const BlockType blockType)
		:StageObjectBase(stage, L"Block"), m_blockType(blockType)
	{}

	Block::Block(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"Block")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);
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
		switch (m_blockType)
		{
		case BlockType::Wall:
			drawComp->SetMeshResource(L"DEFAULT_CUBE");
			drawComp->SetTextureResource(L"Wall01_TX");

			AddTag(L"T_Obstacle");
			AddTag(L"Wall");
			break;
		case BlockType::Floor:
		{
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;

			// スケールに応じたUVを持つCubeを設定
			AdvMeshUtil::CreateCube(1.0f, m_transformData.Scale, vertices, indices);
			m_meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
			drawComp->SetMeshResource(m_meshRes);
			drawComp->SetTextureResource(L"Floor_TX");

			AddTag(L"Floor");
		}
		break;
		default:
			break;
		}

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
	}

	void Block::OnUpdate() {
	}
}