#include "stdafx.h"
#include "Reticle.h"
#include "Utility.h"

namespace basecross {
	void Reticle::OnCreate()
	{
		Col4 color(1.0f, 1.0f, 0.0f, 1.0f); // �Ə��̐F
		float size = 50.0f; // �Ə��̑傫���i�{���̓����o�ϐ��ɂ��ăR���X�g���N�^�Őݒ肷��ׂ��j
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-0.5f, +0.5f, 0.0f) * size, color, Vec2(0.0f, 0.0f)},
			{Vec3(+0.5f, +0.5f, 0.0f) * size, color, Vec2(1.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f, 0.0f) * size, color, Vec2(0.0f, 1.0f)},
			{Vec3(+0.5f, -0.5f, 0.0f) * size, color, Vec2(1.0f, 1.0f)},
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"reticle");

		SetAlphaActive(true); // �����F�ɑΉ�������
	}

	void Reticle::OnUpdate()
	{
		const auto& stage = GetStage(); // �X�e�[�W���擾����

		auto targetPos = target->GetComponent<Transform>()->GetPosition(); // �^�[�Q�b�g�̃��[���h���W���擾����
		auto target2DPos = Utility::ConvertWorldToScreen(stage->GetView(), targetPos); // �^�[�Q�b�g�̍��W���X�N���[�����W�ɕϊ�����

		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(target2DPos); // �Ə����^�[�Q�b�g�̓񎟌����W�Ɉړ�������
	}
}