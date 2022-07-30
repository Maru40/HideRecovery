#pragma once
#include "stdafx.h"

namespace basecross
{
	// ��ʏ㕔�̃X�e�[�W�Z���N�g�^�C�g�������p�̃N���X
	class StageSelectTitle : public GameObject
	{
	public:
		StageSelectTitle(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}