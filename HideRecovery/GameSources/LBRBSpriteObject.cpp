/*!
@file LBRBSpriteObject.cpp
@brief LBRBSpriteObjectÇÃÉNÉâÉXé¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "LBRBSpriteObject.h"

#include "SpriteObject.h"

#include "BuilderVertexPCT.h"
#include "SelectSprite.h"

namespace basecross {

	LBRBSpriteObject::LBRBSpriteObject(const std::shared_ptr<Stage>& stage)
		:LBRBSpriteObject(stage, Type::LB)
	{}

	LBRBSpriteObject::LBRBSpriteObject(const std::shared_ptr<Stage>& stage, const Type& type)
		:GameObject(stage), m_type(type)
	{}

	void LBRBSpriteObject::OnCreate() {
		AddComponent<SelectSprite>();

		auto datas = CreateSpriteDatas(m_type);
		CreateSprites(datas);
	}

	void LBRBSpriteObject::CreateSprites(std::vector<SpriteData>& datas) {
		for (const auto& data : datas) {
			const Vec3& offset = data.offset;
			auto object = GetStage()->AddGameObject<SpriteObject>(data.buildParam);
			object->GetComponent<Transform>()->SetPosition(offset);
			object->SetParent(GetThis<GameObject>());
			m_sprites.push_back(object);
		}
	}

	std::vector<SpriteData> LBRBSpriteObject::CreateSpriteDatas(const LBRBSpriteObject::Type& type) {
		using Type = LBRBSpriteObject::Type;

		//TextureÇîªíf
		std::vector<wstring> textures;
		Vec2 mainOffset;
		switch (type)
		{
		case Type::LB:
			textures.push_back(L"LBBack_TX");
			textures.push_back(L"LB_TX");
			mainOffset = Vec2(+5.0f, 0.0f);
			break;

		case Type::RB:
			textures.push_back(L"RBBack_TX");
			textures.push_back(L"RB_TX");
			mainOffset = Vec2(-5.0f, 0.0f);
			break;
		}

		std::vector<SpriteData> datas = {
			SpriteData(
				Vec2(0.0f, 0.0f),
				Builder::VertexPCTParametor(Vec3(256.0f ,256.0f ,0.0f), 0.35f, Vec2(256.0f, 256.0f), textures[0])
			),
			SpriteData(
				mainOffset,
				Builder::VertexPCTParametor(Vec3(64.0f ,64.0f ,0.0f), 0.65f, Vec2(64.0f, 64.0f), textures[1])
			),
		};

		return datas;
	}

}