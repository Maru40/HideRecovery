
/*!
@file RectDraw.cpp
@brief RectDraw�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "RectDraw.h"

#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	RectDraw::RectDraw(
		const std::shared_ptr<GameObject>& objPtr, 
		const maru::Rect& rect, 
		const std::wstring& texture,
		const Col4& color
	):
		Component(objPtr),
		m_rect(rect),
		m_texture(texture),
		m_color(color)
	{}

	void RectDraw::OnCreate() {
		m_selfTransform = GetGameObject()->GetComponent<Transform>();

		CreateRectObject();
	}

	void RectDraw::OnUpdate() {
		if (auto rectObjectTransform = m_rectObjectTransform.lock()) {
			rectObjectTransform->SetPosition(m_selfTransform.lock()->GetPosition());
			rectObjectTransform->SetForward(m_selfTransform.lock()->GetForward());
			rectObjectTransform->SetScale(m_selfTransform.lock()->GetScale());
		}
	}

	void RectDraw::CreateRectObject() {
		//�I�u�W�F�N�g����
		auto object = GetStage()->Instantiate<GameObject>(m_rect.centerPosition, Quat::Identity());
		object->SetAlphaActive(true);
		
		//���_�ƃC���f�b�N�X�f�[�^�̐���
		constexpr float fScale = 0.8f;
		auto buildParam = Builder::VertexPCTParametor(Vec3(fScale), Vec2(256.0f, 512.0f), m_texture);	//�����`�p�̃Z����ݒ�	//Arrow_TX
		buildParam.color = m_color;
		auto data = Builder::BuilderVertexPCT(buildParam);

		//�\���R���|�[�l���g�̃A�^�b�`
		auto draw = object->AddComponent<PCTStaticDraw>();
		draw->SetOriginalMeshUse(true);
		draw->CreateOriginalMesh(data.m_vertices, data.m_indices);
		draw->SetDepthStencilState(DepthStencilState::Read);
		
		//�e�N�X�`��������Ȃ�e�N�X�`���𒣂�
		if (buildParam.texture != L"") {
			draw->SetTextureResource(buildParam.texture);
		}

		//�傫������
		const auto& width = m_rect.width;
		constexpr float Height = 1.0f;
		const auto& depth = m_rect.depth;
		auto objTrans = object->GetComponent<Transform>();

		objTrans->SetScale(Vec3(width, Height, depth));

		m_rectObject = object;
		m_rectObjectTransform = objTrans;
	}

}