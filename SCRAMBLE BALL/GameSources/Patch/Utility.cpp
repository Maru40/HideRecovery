#include "stdafx.h"
#include "Utility.h"

namespace basecross {
	Vec3 Utility::ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position)
	{
		const auto camera = view->GetTargetCamera();
		const auto viewport = view->GetTargetViewport();
		float w = viewport.Width * 0.5f;  // ��ʂ̕��̔���
		float h = viewport.Height * 0.5f; // ��ʂ̍����̔���

		auto mView = camera->GetViewMatrix(); // �r���[�s��
		auto mProj = camera->GetProjMatrix(); // �v���W�F�N�V����(�ˉe)�s��
		Mat4x4 mScreen( // �X�N���[���s��iBaseCross�p�j
			{ w, 0, 0, 0 },
			{ 0, h, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		);

		Mat4x4 mVPS = mView * mProj * mScreen;

		return Vec3(XMVector3TransformCoord(position, mVPS));
	}
}