
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

	RectDraw::RectDraw(const std::shared_ptr<GameObject>& objPtr, const maru::Rect& rect):
		Component(objPtr),
		m_rect(rect)
	{}

	void RectDraw::OnCreate() {
		m_selfTransform = GetGameObject()->GetComponent<Transform>();

		CreateRectObject();
	}

	void RectDraw::OnUpdate() {
		if (auto rectObjectTransform = m_rectObjectTransform.lock()) {
			rectObjectTransform->SetPosition(m_selfTransform.lock()->GetPosition());
		}
	}

	void RectDraw::CreateRectObject() {
		//�I�u�W�F�N�g����
		auto object = GetStage()->Instantiate<GameObject>(m_rect.centerPosition, Quat::Identity());
		
		//���_�ƃC���f�b�N�X�f�[�^�̐���
		auto buildParam = Builder::VertexPCTParametor(Vec3(1.0f) ,Vec2(0.0f, 0.0f) , L"");	//�����`�p�̃Z����ݒ�
		auto data = Builder::BuilderVertexPCT(buildParam);
		//�\���R���|�[�l���g�̃A�^�b�`
		auto draw = object->AddComponent<PNTStaticDraw>();
		draw->SetOriginalMeshUse(true);
		draw->CreateOriginalMesh(data.m_vertices, data.m_indices);

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