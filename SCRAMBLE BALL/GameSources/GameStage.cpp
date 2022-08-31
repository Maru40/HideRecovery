/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"
#include "StageFloor.h"
#include "EnergeyGauge.h"
#include "PlayerHpgauge.h"
#include "Reticle.h"
#include "CountTime.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -20.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}



	void GameStage::OnCreate() {
		GameStageBase::OnCreate();
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
		}
		catch (...) {
			throw;
		}

		// �Q�[���I�u�W�F�N�g�̒ǉ�
		
		// �v���C���[�i�����́j�̕\��
		auto player = AddGameObject<Player>();
		// �n�ʁi�����́j�̕\��
		AddGameObject<StageFloor>();
		// �G�l���M�[�Q�[�W�̒ǉ�
		AddGameObject<EnergeyGauge>();
		// �v���C���[��HP�Q�[�W�̒ǉ�
		AddGameObject<PlayerHpgauge>();
		// ���e�B�N���̒ǉ�
		AddGameObject<Reticle>(player);
		//// �o�ߎ��Ԃ̒ǉ�
		//AddGameObject<CountTime>();
	}

}
//end basecross