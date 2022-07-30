#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class TitleStage : public  Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;

		std::shared_ptr<Cursor> m_CursorUi;

		bool m_sceneflag;
		//�\�z�Ɣj��
		TitleStage() :Stage(),m_sceneflag(false) {}
		virtual ~TitleStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override;
	};


}
//end basecross

