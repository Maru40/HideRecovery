/*!
@file SpriteObject.cpp
@brief SpriteObject�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SpriteObject.h"

#include "Maruyama/UI/2D/Component/SelectSprite.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �X�v���C�g�I�u�W�F�N�g�{��
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
	/// �X�v���C�g�f�[�^
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