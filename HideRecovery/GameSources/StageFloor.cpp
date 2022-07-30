#include "stdafx.h"
#include "StageFloor.h"

namespace basecross {

	StageFloor::StageFloor(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
		:StageObjectBase(stage, name, texture)
	{ }

	void StageFloor::OnCreate()
	{
		StageObjectBase::OnCreate();

		// �n�ʂ��L���[�u�Œu��
		auto drawComponent = AddComponent<PNTStaticDraw>();
		drawComponent->SetMeshResource(L"DEFAULT_CUBE");
		drawComponent->SetOwnShadowActive(true);
		drawComponent->SetTextureResource(L"Floor_TX");

		// �n�ʂ̈ʒu���Ɋւ�����
		auto transComponent = GetComponent<Transform>();
		transComponent->SetPosition(0.0f, 0.0f, 0.0f); // �ʒu���̐ݒ�
		transComponent->SetScale(20.0f, 1.0f, 20.0f); // �X�P�[���̕ύX

		// �������Z�̒ǉ�
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetFixed(true);

		AddTag(L"StageFloor"); // �n�ʂɃ^�O������

	}

	void StageFloor::OnUpdate()
	{
		// ���ɂȂ�
	}
}

