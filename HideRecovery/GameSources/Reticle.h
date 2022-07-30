#pragma once
#include "stdafx.h"

namespace basecross {
	class Reticle : public GameObject
	{
		std::shared_ptr<GameObject> target; // �Ə�����������Ώۂ̃I�u�W�F�N�g

	public:
		Reticle(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& target)
			: GameObject(stage), target(target) // �R���X�g���N�^�őΏۂ��w�肷��
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
	};
}