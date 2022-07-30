#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageSelect : public  Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;
		bool m_sceneflag;
		//�\�z�Ɣj��
		StageSelect() :Stage(), m_sceneflag(false) {}
		virtual ~StageSelect() {}
		//������
		void OnCreate()override;
		void OnUpdate()override;
	};
}
//end basecross

