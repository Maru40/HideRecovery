/*!
@file   Block.cpp
@brief  ステージを構成するブロッククラス実体
*/

#include "stdafx.h"
#include "Block.h"
#include "../Utility/DataExtracter.h"
#include "../Utility/AdvMeshUtil.h"
#include "GameStageBase.h"

namespace basecross {
	Block::Block(const shared_ptr<Stage>& stage, const BlockType blockType)
		:StageObjectBase(stage, L"Block"), m_blockType(blockType), m_isReactCamera(true)
	{}

	Block::Block(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"Block"), m_isReactCamera(true)
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

		if (tokens.size() > nextIndex + 1) {
			m_isReactCamera = Utility::WStrToBool(tokens[nextIndex + 1]);
		}
	}

	void Block::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetSamplerState(SamplerState::LinearWrap);
		drawComp->SetOwnShadowActive(true);

		switch (m_blockType)
		{
		case BlockType::Wall:
			drawComp->SetMeshResource(L"DEFAULT_CUBE");
			drawComp->SetTextureResource(L"Wall01_TX");

			if (m_isReactCamera) {
				AddTag(L"T_Obstacle");
				AddTag(L"Wall");
			}
			break;
		case BlockType::Floor:
		{
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;

			// スケールに応じたUVを持つCubeを設定
			AdvMeshUtil::CreateCube(4.0f, m_transformData.Scale, vertices, indices);
			m_meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
			drawComp->SetMeshResource(m_meshRes);
			drawComp->SetTextureResource(L"Floor_TX");

			AddTag(L"Floor");

			auto stage = GetTypeStage<GameStageBase>(false);
			if (stage) {
				stage->AddFloorObject(GetThis<Block>());
			}
		}
		break;
		default:
			break;
		}

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);

		// 影の場合UVは関係ないのでDEFAULT_CUBEで十分
		auto shadow = AddComponent<Shadowmap>();
		shadow->SetMeshResource(L"DEFAULT_CUBE");
	}

	void Block::OnUpdate() {
	}
}