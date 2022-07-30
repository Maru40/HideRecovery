/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Skybox2 : public GameObject
	{
	public :
		static const std::map<std::wstring, Vec3> pairs; // �e�N�X�`���L�[(�A�Z�b�g��)�Ɣz�u���W���֘A�}����A�z�z��

	private :
		std::vector<std::shared_ptr<GameObject>> m_planes; // �X�J�C�L���[�u�̂U�ʂ��Ǘ�����

	public :
		Skybox2(const shared_ptr<Stage>& stage)
			: GameObject(stage)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override; // �����ŕ`����s�����߃I�[�o�[���C�h����
	};

}
//end basecross
