/*!
@file SpriteObject.cpp
@brief SpriteObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SpriteObject.h"

#include "Maruyama/UI/2D/Component/SelectSprite.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// スプライトオブジェクト本体
	//--------------------------------------------------------------------------------------

	SpriteObject::SpriteObject(const std::shared_ptr<Stage>& stage, const Builder::VertexPCTParametor& vertexParam)
		:GameObject(stage), m_paramPtr(new Builder::VertexPCTParametor(vertexParam))
	{}

	void SpriteObject::OnCreate() {
		auto build = Builder::BuilderVertexPCT(*m_paramPtr.get());

		auto draw = AddComponent<DrawComp>(build.m_vertices, build.m_indices);
		draw->SetTextureResource(m_paramPtr->texture);
		SetAlphaActive(true);
	}

	//--------------------------------------------------------------------------------------
	/// スプライトデータ
	//--------------------------------------------------------------------------------------

	SpriteData::SpriteData(
		const Vec2& offset,
		const Builder::VertexPCTParametor& buildParam
	) :
		SpriteData(Vec3(offset.x, offset.y, 0.0f), buildParam)
	{}

	SpriteData::SpriteData(
		const Vec3& offset,
		const Builder::VertexPCTParametor& buildParam
	) :
		offset(offset), buildParam(buildParam)
	{}

}