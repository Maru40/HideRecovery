#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class ResultStage : public  Stage{
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;
		bool m_sceneflag;
		int m_sceneMode;
		wstring m_StageName[7] = {
			L"StageS1.csv",
			L"StageS2.csv", // �ŏ��̃X�e�[�W
			L"StageM1.csv",
			L"StageM2.csv",
			L"StageL1.csv",
			L"StageL2.csv",
			L"StageS1.csv",
			//L"StageA.csv", // �ŏ��̃X�e�[�W�ɖ߂�悤�ɂ���
		};
		//�\�z�Ɣj��
		ResultStage() :Stage(), m_sceneflag(false), m_sceneMode(0) {}
		virtual ~ResultStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override;
	};


}
//end basecross

